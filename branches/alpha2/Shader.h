#pragma once
#include "../Utilities/Utilities.h"
#include <vector>
#include <cstring>

class Shader
{
private:
	GLuint mVertexShader;
	GLuint mFragmentShader;
	std::vector<GLenum> mStates;
	GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadShader(GLenum type, const char* filename);

	char* mVSpath;
	char* mFSpath;
	
public:
	GLuint mProgram;
	GLuint mAttrPosition;
	GLuint mAttrUV;
	GLuint mAttrNormal;
	GLuint mUniTime;
	GLuint mUniColor;
	GLuint mUniAmbient;
	GLuint mUniDiffuse;
	GLuint mUniSpecular;
	GLuint mUniTransparent;
	GLuint mUniWorldMatrix;
	GLuint mUniCameraPos;
	GLuint mUniFinalMatrix;
	GLuint mUniTextures[4];
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	int init();
	void addState(GLenum state);
	void enableState();
	void disableState();
	virtual ~Shader();
	static GLenum getStateByString(const char *stateStr);
};
