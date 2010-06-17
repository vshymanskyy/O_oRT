#pragma once

class File{
private:
	FILE* mFile;
	const char* mName;
	bool mWrite, mOpened;

	void Close() {
		assert(mOpened);
		mOpened = false;
		fclose(mFile);
	}

	void Open() {
		const char* mode =	(mWrite)? "w+" : "r";
		mFile = fopen(mName, mode);
		mOpened = (mFile!=NULL);
		assert(mOpened);
	}

public:
	File(const char* name, bool write = false)
		: mName(name)
		, mWrite(write)
	{
		Open();
	}

	~File() {
		Close();
	}

	char* GetPath() {
		const char* p_slash = strrchr(mName,'\\');
		const ptrdiff_t count = p_slash-mName+1;
		char* result = new char[count+1];
		strncpy(result, mName, count);
		result[count] = '\0';
		return result;
	}

	char* GetName() {
		const char* p_slash = strrchr(mName,'\\');
		const char* p_dot = strrchr(mName,'.');
		assert(p_dot>p_slash);
		const ptrdiff_t count = p_dot-p_slash-1;
		char* result = new char[count+1];
		strncpy(result, p_slash+1, count);
		result[count] = '\0';
		return result;
	}

	char* GetExtension() {
		const char* p_dot = strrchr(mName,'.');
		const char* last = mName + strlen(mName);
		const ptrdiff_t count = last-p_dot;
		char* result = new char[count+1];
		strncpy(result, p_dot+1, count);
		result[count] = '\0';
		return result;
	}

	int GetPosition() {
		return ftell(mFile);
	}

	void SetPosition(int p) {
		fseek(mFile, p, SEEK_SET);
	}

	void SetPositionDelta(int p) {
		fseek(mFile, p, SEEK_CUR);
	}

	void SetPositionEnd(int p) {
		fseek(mFile, p, SEEK_END);
	}

	int GetSize() {
		const int current = GetPosition();
		SetPositionEnd(0);
		const int result = GetPosition();
		SetPosition(current);
		return result;
	}

	bool EoF() {
		assert(mOpened);
		return feof(mFile)?true:false;
	}

	real ReadFloat() {
		assert(mOpened);
		float val;
		fscanf(mFile, "%f", &val);
		return (real)val;
	}

	real ReadDouble() {
		assert(mOpened);
		double val;
		fscanf(mFile, "%lf", &val);
		return (real)val;
	}

	int ReadInt() {
		assert(mOpened);
		int val;
		fscanf(mFile, "%d", &val);
		return val;
	}

	char* ReadUntilAnyOf(const char* chars) {
		assert(mOpened);
		char* result = new char[1024];
		char* ptr = result;

		int c ;
		while (!strchr(chars, c = fgetc(mFile))) {
			*(ptr++) = c;
		};
		*ptr = '\0';
		return result;
	}

	char* ReadWord() {
		return ReadUntilAnyOf(" \t\n");
	}

	char* ReadLine() {
		assert(mOpened);
		char* result = new char[1024];
		fgets(result,1023,mFile);
		result[strlen(result)-1] = '\0';
		return result;
	}

	void Write(const char* s) {
		assert(mOpened);
		assert(mWrite);
		fputs(s, mFile);
	}

	void WriteLine(const char* s = "") {
		assert(mOpened);
		assert(mWrite);
		fputs(s, mFile);
		fputs("\n", mFile);
	}

	void WriteFloat(float val) {
		assert(mOpened);
		assert(mWrite);
		fprintf(mFile, "%f", val);
	}

	void WriteInt(int val) {
		assert(mOpened);
		assert(mWrite);
		fprintf(mFile, "%d", val);
	}

};
