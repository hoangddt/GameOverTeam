#pragma once
#include "../Utilities/Utilities.h"
#include <vector>

class Shader
{
private:
	GLuint mVertexShader;
	GLuint mFragmentShader;
	std::vector<GLenum> mStates;
	GLuint loadProgram(GLuint vertexShader, GLuint fragmentShader);
	GLuint loadShader(GLenum type, const char* filename);
public:
	GLuint mProgram;
	GLuint mAttrPosition;
	GLuint mAttrUV;
	GLuint mAttrNormal;
	GLuint mUniColor;
	GLuint mUniAmbient;
	GLuint mUniDiffuse;
	GLuint mUniSpecular;
	GLuint mUniTransparent;
	GLuint mUniWorldMatrix;
	GLuint mUniCameraPos;
	GLuint mUniFinalMatrix;
	GLuint mUniTextures[4];
	Shader();
	int init(const char* vertexShaderPath, const char* fragmentShaderPath);
	void addState(GLenum state);
	void enableState();
	void disableState();
	virtual ~Shader();
	static GLenum getStateByString(const char *stateStr);
};