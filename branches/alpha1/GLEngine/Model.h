#pragma once
#include <GLES2\gl2.h>
#include "Vector3.h"

class Model
{
public:
	int mCountOfIndices;
	int mCountOfMaterials;
	float mTransparent;
	int* mFirstArr;
	int* mCountArr;
	Vector3* mAmbientArr;
	Vector3* mDiffuseArr;
	Vector3* mSpecularArr;
	GLuint vbo;
	GLuint ibo;

	void init(const char* modelFile);
	Model(void);
	~Model(void);
};
