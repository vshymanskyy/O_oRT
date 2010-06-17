#include "slFileObj.h"

const int numCommands = 4;
char*  commandList[numCommands] = { "v", "vt", "f", "l" };

bool slFileObj::Load() {
	Reset();
	ScenePtr = mScene;

	FILE*  infile = fopen(mFilename, "r");
	FileLineNumber = 0;

	if (!infile) {
		fprintf(stderr, "LoadObjFile: Unable to open file: %s\n", mFilename);
		return false;
	}

	char inbuffer[1026];
	while (true) {
		if (!fgets(inbuffer, 1026, infile)) {
			fclose(infile);
			PrintCmdNotSupportedErrors(stderr);
			return true;
		}

		FileLineNumber++;

		char*  findStart = Preparse(inbuffer);
		if (findStart == 0 || (*findStart) == '#') {
			continue;	//Ignore if a comment or a blank line
		}

		bool parseErrorOccurred = false;

		char theCommand[17];
		int scanCode = sscanf(inbuffer, "%16s", theCommand);
		if (scanCode != 1) {
			parseErrorOccurred = true;
		}

		int cmdNum = GetCommandNumber(theCommand);
		if (cmdNum == -1) {
			AddUnsupportedCmd(theCommand);
			continue;
		}

		char*  args = ScanForSecondField(findStart);
		bool ok = true;
		switch (cmdNum) {
			case 0: //'v' command
				{
					vec4*  vertData = Vertices.Add();
					ok = ReadVectorR4Hg(args, vertData);
				}
				break;
			case 1: //"vt" command
				{
					vec2*  texData = TextureCoords.Add();
					ok = ReadTexCoords(args, texData);
				}
				break;
			case 2: //The 'f' command
				{
					ok = ProcessFace(args , mDefaultMat);
				}
				break;
			case 3: //'l' command
				UnsupportedLines();
				break;
			default:
				parseErrorOccurred = true;
				break;
		}

		if (!ok) {
			parseErrorOccurred = true;
		}
	}
}

char *slFileObj::Preparse(char* inbuf) {

	//Change white space to real spaces
	char*  s;
	for (s = inbuf; *s != 0; s++) {
		if (*s == '\t' || *s == '\n') {
			*s = ' ';
		}
	}

	return ScanForNonwhite(inbuf);
}

char *slFileObj::ScanForSecondField(char* inbuf) {
	char*  s;
	for (s = inbuf; (*s) != ' ' && (*s) != 0; s++) { }

	return ScanForNonwhite(s);
}

//=============================================================================
//   Return zero (null pointer) if no non-white found
//=============================================================================
char *slFileObj::ScanForNonwhite(char* inbuf) {
	char*  s;
	for (s = inbuf; (*s) != 0; s++) {
		if ((*s) != ' ') {
			return s;
		}
	}

	return 0;
}

//=============================================================================
//   Scan for white space (or end of string)
//=============================================================================
char *slFileObj::ScanForWhite(char* inbuf) {
	char*  s;
	for (s = inbuf; (*s) != 0 && (*s) != ' '; s++) {
		continue;
	}

	return s;
}

//=============================================================================
//   Return zero (null pointer) if no white space or slash found
//=============================================================================
char *slFileObj::ScanForWhiteOrSlash(char* inbuf) {
	char*  s;
	for (s = inbuf; (*s) != ' ' && (*s) != '/'; s++) {
		if ((*s) == 0) {
			return 0;
		}
	}

	return s;
}

int slFileObj::GetCommandNumber(char* cmd) {
	long i;
	for (i = 0; i < numCommands; i++) {
		if (strcmp(cmd, commandList[i]) == 0) {
			return i;
		}
	}

	return -1;	//Command not found
}

bool slFileObj::ReadVectorR4Hg(char* inbuf, vec4* theVec) {
	float X, Y, Z, W;
	int scanCode = sscanf(inbuf, "%f %f %f %f", &X, &Y, &Z, &W);

	theVec->Set(X, Y, Z, W);

	bool retCode = (scanCode == 3 || (scanCode == 4 && theVec->w != 0.0));
	if (scanCode == 3 || theVec->w == 0.0) {
		theVec->w = real(1.0);
	}

	return retCode;
}

bool slFileObj::ReadTexCoords(char* inbuf, vec2* theVec) {
	float depth;
	float X, Y;
	int scanCode = sscanf(inbuf, "%f %f %f", &X, &Y, &depth);
	theVec->Set(X, Y);

	bool retCode = true;
	switch (scanCode) {
		case 1:
			theVec->y = 0.0;
			break;
		case 2:
			break;
		case 3:
			if (depth != 0.0) {
				UnsupportedTextureDepth();
			}
			break;
		default:
			retCode = false;
			break;
	}

	return retCode;
}

bool slFileObj::ProcessFace(char* inbuf , Shader* curMaterial) {

	const int maxNumVerts = 256;

	//Phase 1 : Load vertex data into arrays
	//This array holds the vertex numbers for
	//(a) vertices, (b) texture coordinates, (c) vertex normals.
	long vertNums[3 * maxNumVerts]; //Use -1 for missing values

	bool missingNormals = false;
	bool missingTexCoords = false;

	int i;
	char*  s = inbuf;
	for (i = 0; i < maxNumVerts + 1; i++) {
		s = ScanForNonwhite(s);
		if (s == 0) {
			break;
		}

		if (i >= maxNumVerts) {
			UnsupportedTooManyVerts(maxNumVerts);
			return false;
		}

		long scannedInt;
		int scanCode = sscanf(s, "%ld", &scannedInt);
		if (scanCode == 0) {
			return false;
		}

		//Negative indices refer to counting backwards
		vertNums[3 * i] = (scannedInt > 0) ? scannedInt : Vertices.Size()
				+
			scannedInt;
		if (vertNums[3 * i] < 1 || vertNums[3 * i] > Vertices.Size()) {
			return false;
		}

		s = ScanForWhiteOrSlash(s);
		if ((*s) != '/') {
			vertNums[3 * i + 1] = vertNums[3 * i + 2] = -1; //No texture
															///coords or normal
															///
			missingTexCoords = true;
			missingNormals = true;
			continue;
		}

		if ((*(s + 1)) == '/' || (*(s + 1)) == ' ' || (*(s + 1)) == 0) {
			vertNums[3 * i + 1] = -1;
			missingTexCoords = true;
		} else {
			scanCode = sscanf(s + 1, "%ld", &scannedInt);
			if (scanCode == 0) {
				return false;
			}

			//Negative indices refer to counting backwards
			vertNums[3 * i + 1] =
					(scannedInt > 0) ? scannedInt : TextureCoords.Size() +
				scannedInt;
			if (vertNums[3 * i + 1] < 1
			 ||	 vertNums[3 * i + 1] > TextureCoords.Size()) {
				return false;
			}
		}

		s = ScanForWhiteOrSlash(s + 1);
		if ((*s) != '/') {
			vertNums[3 * i + 2] = -1;	//No normal
			missingNormals = true;
			continue;
		}

		if ((*(s + 1)) == ' ' || (*(s + 1)) == 0) {
			vertNums[3 * i + 2] = -1;
			missingNormals = true;
		} else {
			scanCode = sscanf(s + 1, "%ld", vertNums + 3 * i + 2);
			if (scanCode != 1) {
				return false;
			}

			//Negative indices refer to counting backwards
			vertNums[3 * i + 2] =
					(scannedInt > 0) ? scannedInt : VertexNormals.Size() +
				scannedInt;

			//XXX TO DO: PUT THIS BACK ONCE NORMALS ARE SCANNED IN!
			//if (vertNums[3*i+2]<1 ||
			//vertNums[3*i+2]>VertexNormals.Size()) {
			//return false;
			//}
		}

		s = ScanForWhite(s + 1);
	}

	int numVertsInFace = i;
	if (numVertsInFace < 3) {
		return false;
	}

	//Create the ViewableTriangles
	//Textures: At the moment, we do not support materials, so it does not
	//make any sense to support textures and texture coordinates.
	vec3 vA, vB, vC;

	//Check for perfect parallolgram first
// 	if (numVertsInFace == 4) {
// 		vec3 vD;
// 		vA.SetFromHg(Vertices[vertNums[0] - 1]);
// 		vB.SetFromHg(Vertices[vertNums[3] - 1]);
// 		vC.SetFromHg(Vertices[vertNums[6] - 1]);
// 		vD.SetFromHg(Vertices[vertNums[9] - 1]);
// 		if ((vD - vA) == (vC - vB) && (vB - vA) == (vC - vD)) {
// 
// 			//Add parallelogram
// 			ViewableParallelogram*	vp = new ViewableParallelogram();
// 			vp->Init(vA, vB, vC);
// 			ScenePtr->AddViewable(vp);
// 			return true;
// 		}
// 	}

	//Otherwise, add as (numVertsInFace-2) many triangles.
	int startIdx = 0;
	int stepIdx = 1;
	for (i = 0; i < numVertsInFace - 2; i++) {

		//Add i-th face of (numVertsInFace-2) total triangles.
		int idx2 = NextTriVertIdx(startIdx, &stepIdx, numVertsInFace);
		int idx3 = NextTriVertIdx(idx2, &stepIdx, numVertsInFace);
		int i1 = vertNums[3 * startIdx] - 1;
		int i2 = vertNums[3 * idx2] - 1;
		int i3 = vertNums[3 * idx3] - 1;
		if (i1 == i2 || i1 == i3 || i2 == i3) {

			//Format error: duplicated vertex in planar, convex polygon!
			return false;
		} else {
			vA.SetFromHg(Vertices[i1]);
			vB.SetFromHg(Vertices[i2]);
			vC.SetFromHg(Vertices[i3]);
			startIdx = idx3;
			assert(0 <= idx2 && idx2 < numVertsInFace);
			assert(0 <= idx3 && idx3 < numVertsInFace);

			prTriangle*  vt = new prTriangle(
				vA, vB, vC,
				vec2(0.0,0.0),
				vec2(0.0,1.0),
				vec2(1.0,1.0),
				curMaterial
			);

			ScenePtr->Primitives()->Add(vt);

		}
	}


	return true;
}

int slFileObj::NextTriVertIdx(int start, int* step, int totalNum) {
	int retIdx = start + (*step);
	if (retIdx >= totalNum) {
		retIdx = (retIdx - totalNum) << 1;
		(*step) <<= 1;
	}

	return retIdx;
}

void slFileObj::Reset(void) {
	Vertices.Clear();
	TextureCoords.Clear();

	//VertexNormals.Reset();
	for (long i = 0; i < UnsupportedCmds.Size(); i++) {
		delete UnsupportedCmds[i];
	}

	UnsupportedCmds.Clear();
}

void slFileObj::UnsupportedTextureDepth(void) {
	if ((!UnsupFlagTextureDepth) && ReportUnsupportedFeatures) {
		fprintf(stderr,
				 "slFileObj: Texture Depth - nonzero values not supported. (Line %ld.)\n",
			 FileLineNumber);
		UnsupFlagTextureDepth = true;
	}
}

void slFileObj::UnsupportedLines(void) {
	if ((!UnsupFlagLines) && ReportUnsupportedFeatures) {
		fprintf(stderr,
				 "slFileObj: 'l' commands ignored: lines cannot be rendered.\n");
		UnsupFlagLines = true;
	}
}

void slFileObj::UnsupportedTooManyVerts(int maxVerts) {
	if ((!UnsupFlagTooManyVerts) && ReportUnsupportedFeatures) {
		fprintf(stderr,
				 "slFileObj: Faces with more than %d verts not supported. (Line %ld.)\n",
			 maxVerts, FileLineNumber);
		UnsupFlagTooManyVerts = true;
	}
}

void slFileObj::AddUnsupportedCmd(char* cmd) {
	for (long i = 0; i < UnsupportedCmds.Size(); i++) {
		if (strcmp(cmd, UnsupportedCmds[i]) == 0) {
			return;
		}
	}

	char*  newstring = new char[strlen(cmd) + 1];
	strcpy(newstring, cmd);
	UnsupportedCmds.Add(newstring);
}

void slFileObj::PrintCmdNotSupportedErrors(FILE* outstream) {
	if (!ReportUnsupportedFeatures) {
		return;
	}

	int numUnsupCmds = UnsupportedCmds.Size();
	if (numUnsupCmds > 0) {
		fprintf(outstream,
				 "slFileObj: Unsupported commands in .obj file:  ");
		for (long i = 0; i < numUnsupCmds; i++) {
			if (i != 0) {
				fprintf(outstream, ", ");
			}

			fprintf(outstream, "%s", UnsupportedCmds[i]);
		}

		fprintf(outstream, ".\n");
	}
}
