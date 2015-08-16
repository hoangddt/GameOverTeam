#pragma once
#include "Lib.h"
#include "FileManager.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

class ResourceManager
{
private:
	Model** mpModels;
	uint mCountOfModels;
	uint mCountOfLoadedModels;

	Texture** mpTextures;
	uint mCountOfTextures;
	uint mCountOfLoadedTextures;

	Texture** mpCubetexs;
	uint mCountOfCubetexs;
	uint mCountOfLoadedCubetexs;

	Texture** mpFontTexs;
	uint mCountOfFontTexs;
	uint mCountOfLoadedFontTexs;

	Shader** mpShaders;
	uint mCountOfShaders;
	uint mCountOfLoadedShaders;

	// unnecessary init
	char** mpConfigs;
	uint mCountOfConfigs;

	static ResourceManager* mpInstance;

	void initModels(FILE* ptr_f);
	void initTextures(FILE* ptr_f);
	void initCubetexs(FILE* ptr_f);
	void initFontTexs(FILE* ptr_f);
	void initShaders(FILE* ptr_f);
	void initConfigs(FILE* ptr_f);
	ResourceManager(void);
	ResourceManager(const ResourceManager &){} 
	ResourceManager& operator =(const ResourceManager &){} 
	~ResourceManager(void);
public:
	static ResourceManager* getInstance();
	static void destroyInstance();
	
	Model* getModelById(uint id);
	Shader* getShaderById(uint id);
	Texture* getTextureById(uint id);
	Texture* getCubetexById(uint id);
	Texture* getFontTexById(uint id);
	const char* getConfigById(uint id);
	uint getCountOfModels() const;
	uint getCountOfShaders() const;
	uint getCountOfTextures() const;
	uint getCountOfCubetexs() const;
	uint getCountOfFontTexs() const;
	uint getCountOfConfigs() const;
	uint getCountOfElements() const;
	uint getCountOfLoaded() const;
	bool loadNext();
	void init(const char* rmFile);
};