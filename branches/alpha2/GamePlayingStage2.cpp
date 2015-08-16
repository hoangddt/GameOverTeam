#include "GamePlayingStage2.h"
#include "SoundManager.h"

void GamePlayingStage2::move(Direction direction)
{
	MapBox* obj = mLevelInfo.mMapObject->getCurBox();
	if(!(hasFlag(GAME_PLAYING_MOVING) || obj->isFell()))
	{
		GamePlayingStage1::move(direction);
	}
}

void GamePlayingStage2::checkNewState()
{
	MapBox* obj = mLevelInfo.mMapObject->getCurBox();
	if(obj->isFell())
	{
		removeFlag(GAME_PLAYING_MOVE_OK);
		addFlag(GAME_PLAYING_MOVE_FAIL);
		addFlag(GAME_PLAYING_MOVE_END);
	}
	GamePlayingStage1::checkNewState();
}

void GamePlayingStage2::setLevel(int level)
{
	GamePlayingStage1::setLevel(level);
	mLevelInfo.mMapObject->setSupportFalling(true);
}

void GamePlayingStage2::update()
{
	if(!mLoaded)
	{
		return;
	}
	GamePlayingStage1::update();
	if(mPopupMenu->opened())
	{
		return;// only popup has been update, other will be disabled
	}
	
	if(mIsReady && !hasFlag(GAME_PLAYING_MOVING))
	{
		MapBox* obj = mLevelInfo.mMapObject->getCurBox();
		if(obj->isFell())
		{
			if(!mChecked)
			{
				checkNewState();
				mChecked = true;
			}
		}
		else
		{
			mChecked = false;
			mCharacter->mPostion.y = obj->mPostion.y + CHARACTER_DELTA_Y;
			mCharacter->setVisible(obj->isVisible());
		}
	}
}

GamePlayingStage2::GamePlayingStage2()
{
	mChecked = false;
}