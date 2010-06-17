#pragma once
#include <string.h>

class File{
private:
	FILE* mFile;
	bool mWrite, mOpened;
	const char* mName;

	void Close() {
		assert(mOpened);
		mOpened = false;
		fclose(mFile);
	}

	void Open() {
		const char* mode = (mWrite)? "bw+" : "rb";
		fopen_s(&mFile, mName, mode);
		mOpened = (mFile!=NULL);
		assert(mOpened);
	}

public:
	File(const char* name, bool write = false)
		:mName(name)
		,mWrite(write)
	{
		Open();
	}

	~File() {
		Close();
	}

	char* GetPath() {
		const char* p_slash = strrchr(mName,'\\');
		const int count = p_slash-mName+1;
		char* result = new char[count+1];
		strncpy_s(result, count+1, mName, count);
		result[count] = '\0';
		return result;
	}

	char* GetName() {
		const char* p_slash = strrchr(mName,'\\');
		const char* p_dot = strrchr(mName,'.');
		assert(p_dot>p_slash);
		const int count = p_dot-p_slash-1;
		char* result = new char[count+1];
		strncpy_s(result, count+1, p_slash+1, count);
		result[count] = '\0';
		return result;
	}

	char* GetExtension() {
		const char* p_dot = strrchr(mName,'.');
		const char* last = mName + strlen(mName);
		const int count = last-p_dot;
		char* result = new char[count+1];
		strncpy_s(result, count+1, p_dot+1, count);
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

	template<typename T>
	void Read(T& val) {
		fread((char*)&val, sizeof(T), 1, mFile);
	}

	template<typename T>
	void Write(const T& val) {
		fwrite((char*)&val, sizeof(T), 1, mFile);
	}
};
