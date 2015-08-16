#include "ResourceManager.h"
#include "Lib.h"
#include <stdio.h>
#include "LogHeaderAndroid.h"

ResourceManager* ResourceManager::mpInstance = NULL;
ResourceManager::ResourceManager(void)
{
	mpModels = NULL;
	mpTextures = NULL;
	mpCubetexs = NULL;
	mpShaders = NULL;
	mpConfigs = NULL;

	mCountOfModels = 0;
	mCountOfLoadedModels = 0;

	mCountOfTextures = 0;
	mCountOfLoadedTextures = 0;

	mCountOfCubetexs = 0;
	mCountOfLoadedCubetexs = 0;

	mCountOfFontTexs = 0;
	mCountOfLoadedFontTexs = 0;

	mCountOfShaders = 0;
	mCountOfLoadedShaders = 0;
}
ResourceManager::~ResourceManager(void)
{
	FREE_2D_ARRAY(mpModels, mCountOfModels);
	FREE_2D_ARRAY(mpTextures, mCountOfTextures);
	FREE_2D_ARRAY(mpShaders, mCountOfShaders);
	FREE_2D_ARRAY(mpCubetexs, mCountOfCubetexs);
	FREE_2D_ARRAY(mpFontTexs, mCountOfFontTexs);
	FREE_2D_ARRAY(mpConfigs, mCountOfConfigs);
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
bool ResourceManager::loadNext()
{
	if(mCountOfLoadedModels < mCountOfModels)
	{
		mpModels[mCountOfLoadedModels++]->init();
		return true;
	}
	if(mCountOfLoadedTextures < mCountOfTextures)
	{
		mpTextures[mCountOfLoadedTextures++]->init();
		return true;
	}
	if(mCountOfLoadedCubetexs < mCountOfCubetexs)
	{
		mpCubetexs[mCountOfLoadedCubetexs++]->init();
		return true;
	}
	if(mCountOfLoadedFontTexs < mCountOfFontTexs)
	{
		mpFontTexs[mCountOfLoadedFontTexs++]->init();
		return true;
	}
	if(mCountOfLoadedShaders < mCountOfShaders)
	{
		mpShaders[mCountOfLoadedShaders++]->init();
		return true;
	}
	return false;
}

void ResourceManager::init(const char* rmFile)
{
	FILE *fp = FileManager::getInstance()->openFile(rmFile, "r");
	initModels(fp);
	fscanf(fp, "\n");
	initTextures(fp);
	fscanf(fp, "\n");
	initCubetexs(fp);
	fscanf(fp, "\n");
	initFontTexs(fp);
	fscanf(fp, "\n");
	initShaders(fp);
	fscanf(fp, "\n");
	initConfigs(fp);
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

const char* ResourceManager::getConfigById(uint id)
{
	if (id >= 0 && id < mCountOfConfigs)
	{
		return mpConfigs[id];
	}
	else
	{
		printf("Error: Invalid menu config id: %d\n", id);
		return NULL;
	}
}

Texture* ResourceManager::getFontTexById(uint id)
{
	if (id >= 0 && id < mCountOfFontTexs)
	{
		return mpFontTexs[id];
	}
	else
	{
		printf("Error: Invalid Font Textures id: %d\n", id);
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

uint ResourceManager::getCountOfFontTexs() const
{
	return mCountOfFontTexs;
}

uint ResourceManager::getCountOfCubetexs() const
{
	return mCountOfCubetexs;
}

uint ResourceManager::getCountOfConfigs() const
{
	return mCountOfConfigs;
}

uint ResourceManager::getCountOfElements() const
{
	return mCountOfModels + mCountOfShaders + 
		mCountOfTextures + mCountOfFontTexs + 
		mCountOfCubetexs;
}

uint ResourceManager::getCountOfLoaded() const
{
	return mCountOfLoadedModels + mCountOfLoadedTextures +
		mCountOfLoadedCubetexs + mCountOfLoadedFontTexs +
		mCountOfLoadedShaders;
}

/*-----  Definition private method  ------*/
void ResourceManager::initModels(FILE* ptr_f)
{
	//float countFloat;
	fscanf(ptr_f, "#Models: %u\n", &mCountOfModels);
	printf("#Models: %u\n", mCountOfModels);
	//char mystring[100];
	//fgets(mystring, 100, ptr_f);
	//printf("Whole line: %s\n", mystring);
	mpModels = new Model*[mCountOfModels];
	int index;
	char* path = new char[70];
	for (int i = 0; i < mCountOfModels; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		printf("ID: %d and %s\n", index, path);
		mpModels[index] = new Model(path);
	}
	delete[] path;
}

void ResourceManager::initTextures(FILE* ptr_f)
{
	fscanf(ptr_f, "#2D Textures: %u\n", &mCountOfTextures);
	printf("#2DTextures: %u\n", mCountOfTextures);

	mpTextures = new Texture*[mCountOfTextures];
	int index;
	char *path = new char[70];
	char *type = new char[20];
	for (int i = 0; i < mCountOfTextures; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		fscanf(ptr_f, "TILING %s\n", type);
		printf("ID: %d FILE: %s TILING: %s\n", index, path, type);
		GLuint tiling = Texture::getTilingByString(type);
		mpTextures[index] = new Texture(true, path, tiling);
	}
	delete[] path;
	delete[] type;
}

void ResourceManager::initFontTexs(FILE* ptr_f)
{
	fscanf(ptr_f, "#Font Textures: %d\n", &mCountOfFontTexs);

	mpFontTexs = new Texture* [mCountOfFontTexs];
	int index;
	char *path = new char[50];
	for (int i = 0; i < mCountOfFontTexs; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		mpFontTexs[index] = new Texture(true, path, GL_REPEAT);
	}
	delete[] path;
}

void ResourceManager::initCubetexs(FILE* ptr_f)
{
	// read and create Cube Texture
	fscanf(ptr_f, "#Cube Textures: %d\n", &mCountOfCubetexs);

	mpCubetexs = new Texture*[mCountOfCubetexs];
	int index;
	char *path = new char[70];
	char *type = new char[20];
	for (int i = 0; i < mCountOfCubetexs; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "FILE %s\n", path);
		fscanf(ptr_f, "TILING %s\n", type);

		GLuint tiling = Texture::getTilingByString(type);
		mpCubetexs[index] = new Texture(false, path, tiling);
	}
	delete[] path;
	delete[] type;
}
void ResourceManager::initShaders(FILE* ptr_f)
{
	// read and create Shader
	fscanf(ptr_f, "#Shaders: %d\n", &mCountOfShaders);

	mpShaders = new Shader*[mCountOfShaders];
	int index;
	char *VSpath = new char[70];
	char *FSpath = new char[70];
	for (int i = 0; i < mCountOfShaders; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &index);
		fscanf(ptr_f, "VS %s\n", VSpath);
		fscanf(ptr_f, "FS %s\n", FSpath);

		Shader *tempShader = new Shader(VSpath, FSpath);
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
	}
	delete[] VSpath;
	delete[] FSpath;
}

void ResourceManager::initConfigs(FILE* ptr_f)
{
	fscanf(ptr_f, "#Menu Configs: %d\n", &mCountOfConfigs);

	mpConfigs = new char* [mCountOfConfigs];

	int id;
	char *configFile = new char[70];

	for (int i = 0; i < mCountOfConfigs; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &id);
		fscanf(ptr_f, "FILE %s\n", configFile);

		int len = strlen(configFile);
		mpConfigs[id] = new char[len + 1];
		strcpy(mpConfigs[i], configFile);
	}

	delete[] configFile;
}
