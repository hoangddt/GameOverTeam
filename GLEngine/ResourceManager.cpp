#include "ResourceManager.h"
#include "Lib.h"
#include <stdio.h>

ResourceManager* ResourceManager::mpInstance = NULL;
ResourceManager::ResourceManager(void)
{
	mpModels = NULL;
	mpTextures = NULL;
	mpCubetexs = NULL;
	mpShaders = NULL;
	mCountOfModels = mCountOfTextures = 0;
	mCountOfCubetexs = mCountOfShaders = 0;
}
ResourceManager::~ResourceManager(void)
{
	FREE_2D_ARRAY(mpModels, mCountOfModels);
	FREE_2D_ARRAY(mpTextures, mCountOfTextures);
	FREE_2D_ARRAY(mpShaders, mCountOfShaders);
	FREE_2D_ARRAY(mpCubetexs, mCountOfCubetexs);
}

ResourceManager* ResourceManager::getInstance()
{
	if (mpInstance == NULL)
		mpInstance = new ResourceManager;
	return mpInstance;
}
void ResourceManager::destroyInstance()
{
	SAFE_FREE(mpInstance);
}

/*-----  Definition public method  ------*/
void ResourceManager::init(const char* rmFile)
{
	FILE *fp = FileManager::getInstance()->openFile(rmFile, "r");
	initModels(fp);
	initTextures(fp);
	initCubetexs(fp);
	initShaders(fp);
	FileManager::getInstance()->closeFile(fp);
}

Model* ResourceManager::getModelById(uint id)
{
	if (id >= 0 && id < mCountOfModels)
	{
		return mpModels[id];
	}
	else
	{
		printf("Error: Invalid Model id: %d\n", id);
		return NULL;
	}
}
Shader* ResourceManager::getShaderById(uint id)
{
	if (id >= 0 && id < mCountOfShaders)
	{
		return mpShaders[id];
	}
	else
	{
		printf("Error: Invalid Shader id: %d\n", id);
		return NULL;
	}
}
Texture* ResourceManager::getTextureById(uint id)
{
	if (id >= 0 && id < mCountOfTextures)
	{
		return mpTextures[id];
	}
	else
	{
		printf("Error: Invalid 2DTexture id: %d\n", id);
		return NULL;
	}
}
Texture* ResourceManager::getCubetexById(uint id)
{
	if (id >= 0 && id < mCountOfCubetexs)
	{
		return mpCubetexs[id];
	}
	else
	{
		printf("Error: Invalid CubeTexture id: %d\n", id);
		return NULL;
	}
}
uint ResourceManager::getCountOfModels() const
{
	return mCountOfModels;
}
uint ResourceManager::getCountOfShaders() const
{
	return mCountOfShaders;
}
uint ResourceManager::getCountOfTextures() const
{
	return mCountOfTextures;
}
uint ResourceManager::getCountOfCubetexs() const
{
	return mCountOfCubetexs;
}


/*-----  Definition private method  ------*/
void ResourceManager::initModels(FILE* ptr_f)
{
	// Read and create Model
	fscanf(ptr_f, "#Models: %d\n", &mCountOfModels);

	
	mpModels = new Model*[mCountOfModels];
	for (int i = 0; i < mCountOfModels; ++i)
	{
		int index;
		char* path = new char[50];
		
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		

		Model *tempModel = new Model();
		tempModel->init(path);


		mpModels[index] = tempModel;
		delete[] path;
		// system("pause");
	}

	fscanf(ptr_f, "\n");
}
void ResourceManager::initTextures(FILE* ptr_f)
{
	// read and create 2D Texture
	fscanf(ptr_f, "#2D Textures: %d\n", &mCountOfTextures);

	mpTextures = new Texture*[mCountOfTextures];
	for (int i = 0; i < mCountOfTextures; ++i)
	{
		int index;
		char *path = new char[50];
		char *type = new char[50];

		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		fscanf(ptr_f, "TILING %s\n", type);

		GLuint tiling = Texture::getTilingByString(type);
		Texture *tempTexture = new Texture(true, path);
		tempTexture->init();
		tempTexture->mTiling = tiling;
		mpTextures[index] = tempTexture;

		delete[] path;
		delete[] type;
		// system("pause");
	}

	fscanf(ptr_f, "\n");
}
void ResourceManager::initCubetexs(FILE* ptr_f)
{
	// read and create Cube Texture
	fscanf(ptr_f, "#Cube Textures: %d\n", &mCountOfCubetexs);

	
	mpCubetexs = new Texture*[mCountOfCubetexs];
	for (int i = 0; i < mCountOfCubetexs; ++i)
	{
		int index;
		char *path = new char[50];
		char *type = new char[50];

		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		fscanf(ptr_f, "TILING %s\n", type);

		GLuint tiling = Texture::getTilingByString(type);
		Texture *tempTexture = new Texture(false, path);
		tempTexture->init();
		tempTexture->mTiling = tiling;

		mpCubetexs[index] = tempTexture;

		delete[] path;
		delete[] type;
		// system("pause");
	}

	fscanf(ptr_f, "\n");
}
void ResourceManager::initShaders(FILE* ptr_f)
{
	// read and create Shader
	fscanf(ptr_f, "#Shaders: %d\n", &mCountOfShaders);

	mpShaders = new Shader*[mCountOfShaders];
	for (int i = 0; i < mCountOfShaders; ++i)
	{
		int index;
		char *VSpath = new char[50];
		char *FSpath = new char[50];

		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "VS %s\n", VSpath);
		fscanf(ptr_f, "FS %s\n", FSpath);

		Shader *tempShader = new Shader();
		tempShader->init(VSpath, FSpath);


		int states;
		fscanf(ptr_f, "STATES %d\n", &states);


		for (int j = 0; j < states; ++j)
		{
			char *stateStr = new char[50];
			fscanf(ptr_f, "STATE %s\n", stateStr);
			GLuint state = Shader::getStateByString(stateStr);
			tempShader->addState(state);
			delete[] stateStr;
		}

		
		
		mpShaders[index] = tempShader;

		delete[] VSpath;
		delete[] FSpath;
		// system("pause");
	}
}
