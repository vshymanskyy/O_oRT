#pragma once

#include "HdrImage.h"

// offsets to red, green, and blue components in a data (float) pixel
#define RGBE_DATA_RED    0
#define RGBE_DATA_GREEN  1
#define RGBE_DATA_BLUE   2
// number of floats per pixel
#define RGBE_DATA_SIZE   4

static
void
FloatToRgbe(unsigned char rgbe[4], float red, float green, float blue)
{
	float v;
	int e;

	v = red;
	if (green > v) v = green;
	if (blue > v) v = blue;
	if (v < 1e-32) {
		rgbe[0] = rgbe[1] = rgbe[2] = rgbe[3] = 0;
	} else {
		v = frexp(v,&e) * 256.0f/v;
		rgbe[0] = (unsigned char) (red * v);
		rgbe[1] = (unsigned char) (green * v);
		rgbe[2] = (unsigned char) (blue * v);
		rgbe[3] = (unsigned char) (e + 128);
	}
}

static
void
RgbeToFloat(float *red, float *green, float *blue, unsigned char rgbe[4])
{
	// note: Ward uses ldexp(col+0.5,exp-(128+8))
	// However we wanted pixels in the range [0,1] to map back into the range [0,1].
	float f;
	if (rgbe[3]) {
		f = ldexp(1.0f, rgbe[3]-(128+8));
		*red = rgbe[0] * f;
		*green = rgbe[1] * f;
		*blue = rgbe[2] * f;
	} else {
		*red = *green = *blue = 0.0;
	}
}

static
bool
WriteData(FILE* fp, float *data, int numpixels)
{
	unsigned char rgbe[4];
	while (numpixels-- > 0) {
		FloatToRgbe(rgbe,data[RGBE_DATA_RED], data[RGBE_DATA_GREEN], data[RGBE_DATA_BLUE]);
		data += RGBE_DATA_SIZE;
		fwrite(rgbe, sizeof(rgbe), 1, fp);
	}
	return true;
}

static
bool
ReadData(FILE* fp, float *data, int numpixels)
{
	unsigned char rgbe[4];
	while(numpixels-- > 0) {
		fread(rgbe, sizeof(rgbe), 1, fp);
		RgbeToFloat(&data[RGBE_DATA_RED],&data[RGBE_DATA_GREEN],&data[RGBE_DATA_BLUE],rgbe);
		data += RGBE_DATA_SIZE;
	}
	return true;
}

static
bool
WriteBytesRLE(FILE* fp, unsigned char *data, int numbytes)
{
	const int MINRUNLENGTH = 4;
	int cur, beg_run, run_count, old_run_count, nonrun_count;
	unsigned char buf[2];

	cur = 0;
	while(cur < numbytes) {
		beg_run = cur;
		/* find next run of length at least 4 if one exists */
		run_count = old_run_count = 0;
		while((run_count < MINRUNLENGTH) && (beg_run < numbytes)) {
			beg_run += run_count;
			old_run_count = run_count;
			run_count = 1;
			while((beg_run + run_count < numbytes) && (run_count < 127)
				&& (data[beg_run] == data[beg_run + run_count]))
				run_count++;
		}
		/* if data before next big run is a short run then write it as such */
		if ((old_run_count > 1)&&(old_run_count == beg_run - cur)) {
			buf[0] = 128 + old_run_count;   /*write short run*/
			buf[1] = data[cur];
			fwrite(buf,sizeof(buf[0])*2,1,fp);
			cur = beg_run;
		}
		/* write out bytes until we reach the start of the next run */
		while(cur < beg_run) {
			nonrun_count = beg_run - cur;
			if (nonrun_count > 128)
				nonrun_count = 128;
			buf[0] = nonrun_count;
			fwrite(buf,sizeof(buf[0]),1,fp);
			fwrite(&data[cur],sizeof(data[0])*nonrun_count,1,fp);
			cur += nonrun_count;
		}
		/* write out next run if one was found */
		if (run_count >= MINRUNLENGTH) {
			buf[0] = 128 + run_count;
			buf[1] = data[beg_run];
			fwrite(buf,sizeof(buf[0])*2,1,fp);
			cur += run_count;
		}
	}
	return true;
}

static
bool
WriteDataRLE(FILE* fp, float *data, int scanline_width, int num_scanlines)
{
	unsigned char rgbe[4];
	unsigned char *buffer;

	if ((scanline_width < 8)||(scanline_width > 0x7fff)) {
		/* run length encoding is not allowed so write flat*/
		return WriteData(fp, data, scanline_width*num_scanlines);
	}
	buffer = (unsigned char *)malloc(sizeof(unsigned char)*4*scanline_width);
	if (buffer == NULL) {
		/* no buffer space so write flat */
		return WriteData(fp, data, scanline_width*num_scanlines);
	}
	while(num_scanlines-- > 0) {
		rgbe[0] = 2;
		rgbe[1] = 2;
		rgbe[2] = scanline_width >> 8;
		rgbe[3] = scanline_width & 0xFF;
		if (fwrite(rgbe, sizeof(rgbe), 1, fp) < 1) {
			free(buffer);
			return false;
		}
		for (int i=0;i<scanline_width;i++) {
			FloatToRgbe(rgbe,data[RGBE_DATA_RED],
				data[RGBE_DATA_GREEN],data[RGBE_DATA_BLUE]);
			buffer[i] = rgbe[0];
			buffer[i+scanline_width] = rgbe[1];
			buffer[i+2*scanline_width] = rgbe[2];
			buffer[i+3*scanline_width] = rgbe[3];
			data += RGBE_DATA_SIZE;
		}
		/* write out each of the four channels separately run length encoded */
		/* first red, then green, then blue, then exponent */
		for (int i=0;i<4;i++) {
			if (!WriteBytesRLE(fp, &buffer[i*scanline_width], scanline_width)) {
				free(buffer);
				return false;
			}
		}
	}
	free(buffer);
	return true;
}

static
bool
ReadDataRLE(FILE* fp, float *data, int scanline_width, int num_scanlines)
{
	unsigned char rgbe[4], *scanline_buffer, *ptr, *ptr_end;

	int i, count;
	unsigned char buf[2];

	if ((scanline_width < 8)||(scanline_width > 0x7fff)) {
		/* run length encoding is not allowed so read flat*/
		return ReadData(fp, data, scanline_width*num_scanlines);
	}
	scanline_buffer = NULL;
	/* read in each successive scanline */
	while(num_scanlines > 0) {
		if (fread(rgbe, sizeof(rgbe), 1, fp) < 1) {
			free(scanline_buffer);
			return false;
		}
		if ((rgbe[0] != 2)||(rgbe[1] != 2)||(rgbe[2] & 0x80)) {
			/* this file is not run length encoded */
			RgbeToFloat(&data[0],&data[1],&data[2],rgbe);
			data += RGBE_DATA_SIZE;
			free(scanline_buffer);
			return ReadData(fp, data, scanline_width*num_scanlines-1);
		}
		if ((((int)rgbe[2])<<8 | rgbe[3]) != scanline_width) {
			free(scanline_buffer);
			return false;
		}
		if (scanline_buffer == NULL) {
			scanline_buffer = (unsigned char *)malloc(sizeof(unsigned char)*4*scanline_width);
		}
		if (scanline_buffer == NULL) {
			return false;
		}

		ptr = &scanline_buffer[0];
		/* read each of the four channels for the scanline into the buffer */
		for (i = 0; i < 4; i++) {
			ptr_end = &scanline_buffer[(i+1)*scanline_width];
			while(ptr < ptr_end) {
				if (fread(buf,sizeof(buf[0])*2,1,fp) < 1) {
					free(scanline_buffer);
					return false;
				}
				if (buf[0] > 128) {
					/* a run of the same value */
					count = buf[0]-128;
					if ((count == 0)||(count > ptr_end - ptr)) {
						free(scanline_buffer);
						return false;
					}while(count-- > 0)
						*ptr++ = buf[1];
				} else {
					/* a non-run */
					count = buf[0];
					if ((count == 0)||(count > ptr_end - ptr)) {
						free(scanline_buffer);
						return false;
					}
					*ptr++ = buf[1];
					if (--count > 0) {
						if (fread(ptr,sizeof(*ptr)*count,1,fp) < 1) {
							free(scanline_buffer);
							return false;
						}
						ptr += count;
					}
				}
			}
		}
		/* now convert data from buffer into floats */
		for (i=0;i<scanline_width;i++) {
			rgbe[0] = scanline_buffer[i];
			rgbe[1] = scanline_buffer[i+scanline_width];
			rgbe[2] = scanline_buffer[i+2*scanline_width];
			rgbe[3] = scanline_buffer[i+3*scanline_width];
			RgbeToFloat(&data[RGBE_DATA_RED], &data[RGBE_DATA_GREEN], &data[RGBE_DATA_BLUE],rgbe);
			data += RGBE_DATA_SIZE;
		}
		num_scanlines--;
	}
	free(scanline_buffer);
	return true;
}

static
void
memswap(void* m1, void* m2, size_t n)
{
	char *p = (char*)m1;
	char *q = (char*)m2;
	char tmp;

	while (n--) {
		tmp = *p;
		*p++ = *q;
		*q++ = tmp;
	}
}


static
void
memreverse(void* array, size_t num, size_t size)
{
	char *l = (char *)array;
	char *r = (char *)array + ((num-1) * size);
	for( ; l < r; l = l+size, r = r-size ) {
		memswap(l, r, size);
	}
}

const char* HdrImage::Format = "32-bit_rle_rgbe";

HdrImage::HdrImage(const char* fn)
	: mData		(NULL)
	, mWidth	(0)
	, mHeight	(0)
{
	Load(fn);
}

HdrImage::HdrImage(int width, int height, rgba* data)
	: mData		(data)
	, mWidth	(width)
	, mHeight	(height)
{
	setProperty("FORMAT", Format);
}

bool
HdrImage::Load(const char* fn)
{
	mFlipX = false;
	mFlipY = false;

	FILE* fp = fopen(fn, "rb");
	if (!fp) {
		setSize(16, 16);
		Clear();
		return false;
	}

	char buf[128];

	fgets(buf, sizeof(buf), fp);
	if (strcmp(buf, "#?RADIANCE\n") != 0) {
		fclose(fp);
		setSize(16, 16);
		Clear();
		return false;
	}
	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);
		if (strcmp(buf, "\n") == 0) {
			break;
		}

		if (buf[0] != '#') {
			if (char* nameEnd = strchr(buf, '=')) {
				*nameEnd++ = '\0';
				size_t valLen = strlen(nameEnd);
				if (nameEnd[valLen-1] == '\n') {
					nameEnd[valLen-1] = '\0';
				}
				setProperty(buf, strdup(nameEnd));
			}
		}
	}

	char flip[2];
	char dim[2];
	int size[2];
	fgets(buf, sizeof(buf), fp);
	sscanf(buf, "%c%c %d %c%c %d", &flip[0], &dim[0], &size[0], &flip[1], &dim[1], &size[1]);

	int width = 0;
	int height = 0;
	bool flipX = false;
	bool flipY = false;
	for (int i = 0; i < 2; i++) {
		switch(dim[i]) {
		case 'X':
		case 'x':
			width = size[i];
			flipX = (flip[i]=='-');
			break;
		case 'Y':
		case 'y':
			height = size[i];
			flipY = (flip[i]=='-');
			break;
		}
	}

	if (width <= 0  || height <= 0) {
		fclose(fp);
		setSize(16, 16);
		Clear();
		return false;
	} else {
		setSize(width, height);
		const bool result = ReadDataRLE(fp, (float*)mData, mWidth, mHeight);
		fclose(fp);
		if (result) {
			if (flipX && flipY) {
				FlipXY();
			} else if (flipY) {
				FlipY();
			} else if (flipX) {
				FlipX();
			}
		}
		return result;
	}
}

bool
HdrImage::Save(const char* fn) const
{
	if (!mData) {
		return false;
	}
	if (FILE* fp = fopen(fn, "wb")) {
		fprintf(fp, "#?RADIANCE\n");
		for (List<Property>::Iterator it = mProperties.First(); it != mProperties.End(); ++it) {
			fprintf(fp, "%s=%s\n", mProperties[it].name, mProperties[it].value);
		}
		fprintf(fp, "\n%cY %d %cX %d\n", mFlipY?'-':'+', mHeight, mFlipX?'-':'+', mWidth);

		const bool result = WriteDataRLE(fp, (float*)mData, mWidth, mHeight);
		fclose(fp);
		return result;
	} else {
		return false;
	}
}

void
HdrImage::Clear()
{
	memset(mData, 0, sizeof(rgba)*mWidth*mHeight);
}

void
HdrImage::FlipX()
{
	for (int y = 0; y < mHeight; y++) {
		memreverse(mData+y*mWidth, mWidth, sizeof(rgba));
	}
}

void
HdrImage::FlipY()
{
	memreverse(mData, mHeight, mWidth*sizeof(rgba));
}

void
HdrImage::FlipXY()
{
	memreverse(mData, mWidth*mHeight, sizeof(rgba));
}

void
HdrImage::setSize(int w, int h)
{
	if (w != mWidth || h != mHeight) {
		mWidth = w;
		mHeight = h;

		if (mData) {
			delete[] mData;
		}
		mData = new rgba[mWidth*mHeight];
	}
}

const char*
HdrImage::getProperty(const char* name) const
{
	for (List<Property>::Iterator it = mProperties.First(); it != mProperties.End(); ++it) {
		if (strcmp(mProperties[it].name, name) == 0) {
			return mProperties[it].value;
			break;
		}
	}
	return NULL;
}

void
HdrImage::setProperty(const char* name, const char* value)
{
	for (List<Property>::Iterator it = mProperties.First(); it != mProperties.End(); ++it) {
		if (strcmp(mProperties[it].name, name) == 0) {
			free(mProperties[it].value);
			mProperties.RemoveAt(it);
			break;
		}
	}
	Property p;
	strncpy(p.name, name, 64);
	p.value = strdup(value);
	mProperties.Append(p);
}

void
HdrImage::setProperty(const char* name, double value)
{
	char buff[32];
	sprintf(buff, "%2.5f", value);
	setProperty(name, buff);
}

void
HdrImage::setProperty(const char* name, float value)
{
	char buff[32];
	sprintf(buff, "%2.5f", value);
	setProperty(name, buff);
}

void
HdrImage::setProperty(const char* name, int value)
{
	char buff[32];
	sprintf(buff, "%d", value);
	setProperty(name, buff);
}
