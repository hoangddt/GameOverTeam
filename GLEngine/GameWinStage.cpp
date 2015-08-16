#include "GameWinStage.h"
#include "GameData.h"
#include "SoundManager.h"

void GameWinStage::update()
{
	StageBase::update();

	mSceneManager->mObjects[0]->mRotation.y += 0.01f;
}

GameWinStage::GameWinStage(void)
{
	SoundManager::getInstance()->stop(SOUND_BG_STAGE3);
	GameData::getInstance()->setCurrStage(4);
}

GameWinStage::~GameWinStage(void)
{
}
