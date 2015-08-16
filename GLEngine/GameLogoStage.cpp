#include "GameLogoStage.h"

bool GameLogoStage::completed() const
{
	return mCompleted;
}

void GameLogoStage::update()
{
	if(!mLoaded)
	{
		return;
	}
	clock_t now = clock();
	now *= 1000.0f / CLOCKS_PER_SEC;
	if(now - mStartTime > LOGO_TIMEOUT)
	{
		mCompleted = true;
	}
	StageBase::update();
}

void GameLogoStage::start()
{
	StageBase::start();
	clock_t now = clock();
	now *= 1000.0f / CLOCKS_PER_SEC;
	mStartTime = now;
}

GameLogoStage::GameLogoStage()
{
	mCompleted = false;
	mStartTime = 0;
}