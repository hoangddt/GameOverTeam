#include "LoadingScreen.h"

void LoadingScreen::setPercent(float percent)
{
	mProcessBar->setPosition(
		mProcessBarX0 + 
		percent * (mProcessBarX1 - mProcessBarX0) / 100.0f - 
		DESIGN_SCREEN_WIDTH, 0.0f);
}

LoadingScreen::LoadingScreen(const char* configFile)
	:Control2D()
{
	FileManager* fileManager = FileManager::getInstance();
	FILE* fp = fileManager->openFile(configFile, "r");

	char* fileName = new char[70];

	// load model
	fscanf(fp, "MODEL %s\n", fileName);
	mModel = new Model(fileName);
	mModel->init();

	// load two textures for this and processbar
	fscanf(fp, "TEXTURE\n");
	fscanf(fp, "BG_FILE %s\n", fileName);
	mTexture = new Texture(true, fileName);
	mTexture->init();
	fscanf(fp, "PR_FILE %s\n", fileName);
	Texture* texture = new Texture(true, fileName);
	texture->init();
	delete[] fileName;

	// load shader
	char* vsFile = new char[70];
	char* fsFile = new char[70];
	fscanf(fp, "SHADER\n");
	fscanf(fp, "VS %s\n", vsFile);
	fscanf(fp, "FS %s\n", fsFile);
	mShader = new Shader(vsFile, fsFile);
	mShader->init();
	delete[] vsFile;
	delete[] fsFile;

	// load processbar coords
	fscanf(fp, "PROCESSBAR %d %d\n", &mProcessBarX0, &mProcessBarX1);

	fileManager->closeFile(fp);

	mObject = new Game2DObject(mModel, mShader, mTexture);
	mObject->mVisibility = VISIBILITY;
	mObject->setFullScreen();
	mObject->init();

	mProcessBar = new Game2DObject(mModel, mShader, texture);
	mProcessBar->mVisibility = VISIBILITY;
	mProcessBar->setFullScreen();
	mProcessBar->init();

	setPercent(0.0f);
}

void LoadingScreen::render()
{
	mProcessBar->render();
	Control2D::render();
}

LoadingScreen::~LoadingScreen()
{
	SAFE_FREE(mModel);
	SAFE_FREE(mTexture);
	SAFE_FREE(mShader);
	SAFE_FREE(mProcessBar->mTexture);
	SAFE_FREE(mProcessBar);
}