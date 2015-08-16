#include "StageManager.h"

StageManager* StageManager::mInstance = NULL;

StageManager* StageManager::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new StageManager();
	}
	return mInstance;
}

void StageManager::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void StageManager::setStage(Stage stage)
{
	switch(stage)
	{
	case STAGE_LOADING:
		break;
	case STAGE_MAINMENU:
		break;
	case STAGE_PLAYING1:
		{
			SAFE_FREE(mStageInstace);
			GamePlayingStage1* playingStage1 = new GamePlayingStage1();
			playingStage1->init("Stage1/RM.txt", "Stage1/SM.txt", "Stage1/stage.txt", 1);
			mStageInstace = playingStage1;
			break;
		}
	case STAGE_PLAYING2:
		break;
	case STAGE_PLAYING3:
		break;
	}
}

void StageManager::render()
{
	if(mStageInstace != NULL)
	{
		mStageInstace->render();
	}
}

void StageManager::update()
{
	if(mStageInstace != NULL)
	{
		mStageInstace->update();
	}
}

void StageManager::destroy()
{
	SAFE_FREE(mStageInstace);
}

StageManager::StageManager(void)
{
	mStageInstace = NULL;
}

StageManager::~StageManager(void)
{
	destroy();
}
