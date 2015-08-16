#include "GameMapStage.h"

bool GameMapStage::isStage1Press() const
{
	return mSceneManager->mMenus[0]->isButtonPress(1);
}

bool GameMapStage::isStage2Press() const
{
	return mSceneManager->mMenus[0]->isButtonPress(2);
}

bool GameMapStage::isStage3Press() const
{
	return mSceneManager->mMenus[0]->isButtonPress(3);
}