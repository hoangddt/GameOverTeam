#include "GameMapStage.h"
#include "GameData.h"

bool GameMapStage::isStage1Press() const
{
	return mSceneManager->mMenus[0]->isButtonPress(1);
}

bool GameMapStage::isStage2Press() const
{
	if(GameData::getInstance()->getCurrStage() < 2)
	{
		return false;
	}
	return mSceneManager->mMenus[0]->isButtonPress(2);
}

bool GameMapStage::isStage3Press() const
{
	if(GameData::getInstance()->getCurrStage() < 3)
	{
		return false;
	}
	return mSceneManager->mMenus[0]->isButtonPress(3);
}

void GameMapStage::init(const char* rmFile, 
						const char* smFile, 
						const char* bgSoundId)
{
	StageBase::init(rmFile, smFile, bgSoundId);
	for(int i = 0; i < mSceneManager->mCountOfMenus; i++)
	{
		mSceneManager->mMenus[i]->setVisible(COLLAPSED);
	}

	int currentState = GameData::getInstance()->getCurrStage();
	switch(currentState)
	{
	case 0:
	case 1:
		mSceneManager->mMenus[RES_MAPSTAGE1_ID]->setVisible(VISIBILITY);
		break;
	case 2:
		mSceneManager->mMenus[RES_MAPSTAGE2_ID]->setVisible(VISIBILITY);
		break;
	case 3:
		mSceneManager->mMenus[RES_MAPSTAGE3_ID]->setVisible(VISIBILITY);
		break;
	case 4:
		mSceneManager->mMenus[RES_MAPSTAGE4_ID]->setVisible(VISIBILITY);
		break;
	}
}

GameMapStage::GameMapStage(): StageBase()
{
	
}