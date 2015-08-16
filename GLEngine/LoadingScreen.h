#pragma once
#include "FileManager.h"
#include "Control2D.h"

class LoadingScreen: public Control2D
{
private:
	Game2DObject* mProcessBar;
	Model* mModel;
	Shader* mShader;
	Texture* mTexture;
	int mProcessBarX0;
	int mProcessBarX1;
public:
	void setPercent(float percent);
	virtual void render();
	LoadingScreen(const char* configFile);
	virtual ~LoadingScreen();
};