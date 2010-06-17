#pragma once
#include "slBase.h"

class slFileObj: public slBase{

	const char* mFilename;

public:
	slFileObj(Scene* scene, const char* filename);

	//The "Load()" routines reads from the file and includes whatever it
	//knows how to process into the scene. (If the scene already includes
	//items, they are left unchanged.)
	bool Load();

private:
	bool ReportUnsupportedFeatures;
	bool UnsupFlagTextureDepth;
	bool UnsupFlagTooManyVerts;
	bool UnsupFlagLines;
	long FileLineNumber;

private:
	Scene*  ScenePtr;
	void Reset(void);

	static char*  Preparse(char* inbuf);
	static char*  ScanForNonwhite(char* inbuf);
	static char*  ScanForWhite(char* inbuf);
	static char*  ScanForWhiteOrSlash(char* inbuf);
	static char*  ScanForSecondField(char* inbuf);
	static int GetCommandNumber(char* cmd);
	static bool ReadVectorR4Hg(char* inbuf, vec4* theVec);
	bool ReadTexCoords(char* inbuf, vec2* theVec);
	bool ProcessFace(char* inbuf , Shader* curMaterial);
	static int NextTriVertIdx(int start, int* step, int totalNum);

	void UnsupportedTextureDepth(void);
	void UnsupportedLines(void);
	void UnsupportedTooManyVerts(int maxVerts);
	void AddUnsupportedCmd(char* cmd);
	void PrintCmdNotSupportedErrors(FILE* outstream);

	Array<vec4> Vertices;	//Vertices in homogenous format
	Array<vec2> TextureCoords;	//Texture coordinates not supported yet
	Array<vec3> VertexNormals;	//Vertex normals not supported yet

	Array<char *> UnsupportedCmds;

};

inline slFileObj::slFileObj(Scene* scene, const char* filename)
	:slBase(scene)
{
	mFilename = filename;
	ReportUnsupportedFeatures = true;
	UnsupFlagTextureDepth = false;
	UnsupFlagTooManyVerts = false;
	UnsupFlagLines = false;
}
