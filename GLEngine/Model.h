#pragma once
#include <GLES2\gl2.h>

class Model
{
public:
	int mCountOfIndices;
	GLuint vbo;
	GLuint ibo;
	void init(const char* modelFile);
	Model(void);
	~Model(void);
};
