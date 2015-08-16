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
	Texture** mpTextures;
	uint mCountOfTextures;
	Texture** mpCubetexs;
	uint mCountOfCubetexs;
	Shader** mpShaders;
	uint mCountOfShaders;
	static ResourceManager* mpInstance;

	void initModels(FILE* ptr_f);
	void initTextures(FILE* ptr_f);
	void initCubetexs(FILE* ptr_f);
	void initShaders(FILE* ptr_f);
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
	uint getCountOfModels() const;
	uint getCountOfShaders() const;
	uint getCountOfTextures() const;
	uint getCountOfCubetexs() const;
	void init(const char* rmFile);
};