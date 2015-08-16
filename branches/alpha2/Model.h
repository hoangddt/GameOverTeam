#pragma once
#include <GLES2\gl2.h>
#include "../Utilities/Vector3.h"

class Model
{
public:
	int mCountOfIndices;
	int mCountOfMaterials;
	float mTransparent;
	int* mFirstArr;
	int* mCountArr;
	char* mModelFile;
	Vector3* mAmbientArr;
	Vector3* mDiffuseArr;
	Vector3* mSpecularArr;
	GLuint vbo;
	GLuint ibo;

	void init();
	Model(const char *modelFile);
	~Model(void);
};
