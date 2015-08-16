#include "GamePlayingStage1.h"
#include <time.h>
#include <math.h>
#include "Map.h"
#include "ControlButton.h"
#include "SoundManager.h"
#include "PlayingStageDrawer.h"
#include "GameData.h"
#include "Game2DObject.h"
#include "LogHeaderAndroid.h"
#include "Lib.h"

bool GamePlayingStage1::hasFlag(uint flag)
{
	return (mState & flag) == flag;
}

void GamePlayingStage1::addFlag(uint flag)
{
	mState |= flag;
}

void GamePlayingStage1::removeFlag(uint flag)
{
	mState &= ~flag;
}

void GamePlayingStage1::move(Direction direction)
{
	printf("GamePlayingStage1::move function\n");
	MapObject* mapObj = mLevelInfo.mMapObject;
	Vector2 curPos = mapObj->getCurrentPosition();

	if(curPos.x <= 0 || !mapObj->isValidCurPosition())
	{
		return;
	}

	curPos.x--;

	switch(direction)
	{
	case GAME_MOVE_LEFT:// left
		if(curPos.y <= 0)
		{
			return;
		}
		transTo(curPos.x, curPos.y - 1, true);
		mapObj->setCharacterPosition(curPos.x, curPos.y - 1);
		break;
	case GAME_MOVE_MID:// mid
		transTo(curPos.x, curPos.y, true);
		mapObj->setCharacterPosition(curPos.x, curPos.y);
		break;
	case GAME_MOVE_RIGHT:// right
		if(curPos.y >= mLevelInfo.mMap[1] - 1)
		{
			return;
		}
		transTo(curPos.x, curPos.y + 1, true);
		mapObj->setCharacterPosition(curPos.x, curPos.y + 1);
		break;
	}
}

void GamePlayingStage1::showPath(bool init)
{
	MapObject* mapObj = mLevelInfo.mMapObject;
	clock_t now = clock();
	now *= 1000.0f / CLOCKS_PER_SEC;

	if(init)
	{
		mStartTime = now;
		mState = GAME_PLAYING_SHOWPATH;
		mapObj->setState(MAP_SHOW_PATH);
	}
	else
	{
		if((mShowPathTimeout > 0) && (now - mStartTime > mShowPathTimeout))
		{
			mapObj->setState(MAP_NORMAL);
			ready();
		}
	}
}

void GamePlayingStage1::ready()
{
	mState = GAME_PLAYING_MOVE_OK;
	mLevelInfo.mMapObject->startGame();
	mCharacter->setVisible(true);
	mSceneManager->mMenus[RES_PAUSE_BUTTON_ID]->setVisible(VISIBILITY);

	mIsReady = true;
}

void GamePlayingStage1::start()
{
	StageBase::start();
	mCharacter = mSceneManager->mObjects[RES_CHARACTER_ID];
	mSceneManager->mMenus[RES_PAUSE_BUTTON_ID]->setVisible(COLLAPSED);
	
	int availableStage = GameData::getInstance()->getCurrStage();
	if(availableStage == 0)
	{
		addFlag(GAME_PLAYING_POPUP);
		mHasShowHelp = true;
		GameData::getInstance()->setCurrStage(1);
		mPopupMenu->show(POPUP_GUIDE1);
	}
	else if(availableStage < mStageId)
	{
		addFlag(GAME_PLAYING_POPUP);
		mHasShowHelp = true;
		mPopupMenu->show((PopUpList)(POPUP_GUIDE1 + mStageId - 1));
		GameData::getInstance()->setCurrStage(mStageId);
	}
	else
	{
		setLevel(1);
	}
}

void GamePlayingStage1::setLevel(int level)
{
	mState = 0;
	mSceneManager->mMenus[RES_PAUSE_BUTTON_ID]->setVisible(COLLAPSED);
	mCharacter->setVisible(false);
	mLevelInfo = mStageInfo->getLevelInfo(level);
	mPopupMenu->show(POPUP_NONE);

	Vector2 pos = mLevelInfo.mMapObject->getCurrentPosition();
	GameObject* obj = mLevelInfo.mMapObject->getBox(pos.x - 1, pos.y);

	mCharacter->mPostion.x = obj->mPostion.x + obj->getActualWidth() / 2.0f - 0.54f;
	mCharacter->mPostion.z = obj->mPostion.z + obj->getActualHeight() / 2.0f - 0.2f;
	mCharacter->mPostion.y = obj->mPostion.y + CHARACTER_DELTA_Y;

	mDrawer->setLevel(level);

	showPath(true);
}

int GamePlayingStage1::getCountOfLevels() const
{
	return mStageInfo->getCountOfLevels();
}

void GamePlayingStage1::nextLevel()
{
	setLevel(mLevelInfo.mLevel + 1);
}

int GamePlayingStage1::getLevel() const
{
	return mLevelInfo.mLevel;
}

void GamePlayingStage1::checkNewState()
{
	// gameover
	if(hasFlag(GAME_PLAYING_MOVE_FAIL))
	{
		if(!mIsShowPathAtGameOver)
		{
			mIsWaitForDisplay = true;
			mIsShowPathAtGameOver = true;
		}
		if(mIsWaitForDisplay)
		{
			updatePathDisplay();
		}
		else
		{
			mIsReady = false;
			if(!hasFlag(GAME_PLAYING_POPUP))
			{
				addFlag(GAME_PLAYING_POPUP);
				mPopupMenu->show(POPUP_GAMEOVER);
			}
			if(mPopupMenu->isMainMenu(POPUP_GAMEOVER))
			{
				addFlag(GAME_PLAYING_GOTO_MAIN);
				mPopupMenu->close();
			}
			else if(mPopupMenu->isTryAgain(POPUP_GAMEOVER))
			{
				//addFlag(GAME_PLAYING_TRY_AGAIN);
				setLevel(1);
				mPopupMenu->close();
				mIsWaitForDisplay = false;
				mIsShowPathAtGameOver = false;
			}
		}
	}
	// level up/next stage
	else if(hasFlag(GAME_PLAYING_MOVE_END))
	{
		if(hasFlag(GAME_PLAYING_POPUP))
		{
			return;
		}
		mIsReady = false;
		addFlag(GAME_PLAYING_POPUP);
		// next stage
		if(mLevelInfo.mLevel >= mStageInfo->getCountOfLevels())
		{
			mPopupMenu->show(POPUP_WINSTAGE, 1500);
		}
		// level up
		else
		{
			mPopupMenu->show(POPUP_LEVELUP, 1500);
		}
	}
	// pause menu
	else if(mPopupMenu->mCurrentPopup == POPUP_PAUSE)
	{
		if(mPopupMenu->isMainMenu(POPUP_PAUSE))
		{
			removeFlag(GAME_PLAYING_MOVE_OK);
			addFlag(GAME_PLAYING_MOVE_END);
			addFlag(GAME_PLAYING_MOVE_FAIL);
			addFlag(GAME_PLAYING_GOTO_MAIN);
			SoundManager::getInstance()->stop(mBackgroundSoundId);
			mPopupMenu->close();
		}
		else if(mPopupMenu->isResume(POPUP_PAUSE))
		{
			ControlButton::getInstance()->mState = BUTTON_NONE;
			mPopupMenu->close();
			mCharacter->setVisible(true);
		}
	}
}

void GamePlayingStage1::transTo(uint row, uint col, bool init)
{
	static float vy;
	static float vx;
	static float vz;

	static float time;
	static float alpha = DEGREES_TO_RADIANS(60);

	static float startX;
	static float startY;
	static float startZ;

	static float signX;
	static float signZ;

	static float desZ;
	static float desX;

	if(init)
	{
		MapObject* mapObj = mLevelInfo.mMapObject;
		GameObject* obj = mapObj->getCurBox();
		Vector2 cPos = mapObj->getCurrentPosition();

		float deltaZ = (row - cPos.x) * obj->getActualHeight();
		float deltaX = (col - cPos.y) * obj->getActualWidth();

		signZ = row > cPos.x ? 1 : (row < cPos.x ? -1 : 0);
		signX = col > cPos.y ? 1 : (col < cPos.y ? -1 : 0);

		float lmax = sqrt(deltaZ * deltaZ + deltaX * deltaX );
		vy = sqrt((MATH_GRAVITY * lmax) / sin(2.0f * alpha));
		vx = sqrt(fabs(MATH_GRAVITY * deltaX) / sin(2.0f * alpha));
		vz = sqrt(fabs(MATH_GRAVITY * deltaZ) / sin(2.0f * alpha));

		time = 0.0f;

		startX = mCharacter->mPostion.x;
		startY = mCharacter->mPostion.y;
		startZ = mCharacter->mPostion.z;
		desX = startX + deltaX;
		desZ = startZ + deltaZ;

		SoundManager::getInstance()->play(SOUND_JUMPSTART);

		addFlag(GAME_PLAYING_MOVING);
	}

	time += 0.065f;

	bool complete = true;

	float newY = startY + vy * time * sin(alpha) - 0.5 * MATH_GRAVITY * time * time;
	float newX = startX + signX * vx * time * cos(alpha);
	float newZ = startZ + signZ * vz * time * cos(alpha);

	// jump y
	if(newY < startY)
	{
		mCharacter->mPostion.y = startY;
	}
	else
	{
		mCharacter->mPostion.y = newY;
		complete = false;
	}
	
	// move x
	if(fabs(mCharacter->mPostion.x - desX) <= fabs(newX - desX))
	{
		mCharacter->mPostion.x = desX;
	}
	else
	{
		mCharacter->mPostion.x = newX;
		complete = false;
	}
	
	// move z
	if(fabs(mCharacter->mPostion.z - desZ) <= fabs(newZ - desZ))
	{
		mCharacter->mPostion.z = desZ;
	}
	else
	{
		mCharacter->mPostion.z = newZ;
		complete = false;
	}
	
	// jump completed!
	if(complete)
	{
		//removeFlag(GAME_PLAYING_MOVING);
		addFlag(GAME_PLAYING_JUMP_END);

		MapObject* mapObj = mLevelInfo.mMapObject;
		Vector2 curPos = mapObj->getCurrentPosition();

		// whenmove right, add move_ok and remove move_fail,
		// then check for finish current level yet
		if(mapObj->isValidCurPosition())
		{
			addFlag(GAME_PLAYING_MOVE_OK);
			removeFlag(GAME_PLAYING_MOVE_FAIL);
			if(curPos.x == 0)
			{
				addFlag(GAME_PLAYING_MOVE_END);
			}
		}
		// when move wrong, add move_fail, remove move_ok
		// and finish current level by add move_end
		else
		{
			addFlag(GAME_PLAYING_MOVE_FAIL);
			addFlag(GAME_PLAYING_MOVE_END);
			removeFlag(GAME_PLAYING_MOVE_OK);
		}
 
		SoundManager::getInstance()->play(SOUND_JUMPEND);

		//checkNewState();

		mapObj->getCurBox()->startVibrateEffect();
	}
}

void GamePlayingStage1::init(const char* rmFile, 
							 const char* smFile, 
							 const char* stageFile,
							 const char* bgSoundId)
{
	StageBase::init(rmFile, smFile, bgSoundId);

	StageInfo::destroyInstance();
	mStageInfo = StageInfo::getInstance();
	mStageInfo->init(stageFile);

	mPopupMenu = new PopUpMenu(
		mSceneManager->mMenus[0],
		mSceneManager->mMenus[1],
		mSceneManager->mMenus[2],
		mSceneManager->mMenus[3],
		mSceneManager->mMenus[5],
		mSceneManager->mMenus[6],
		mSceneManager->mMenus[7]);
	mPopupMenu->show(POPUP_NONE);

	mDrawer->init(
		mResourceManager->getModelById(RES_LEVELINFO_ID), 
		mResourceManager->getShaderById(0), 
		mResourceManager->getConfigById(0));

	Game2DObject* pause = mSceneManager->mMenus[RES_PAUSE_BUTTON_ID]->mObject;
	pause->setSize(DESIGN_SCREEN_WIDTH, 128);
	pause->setPosition(0, (128.0f - DESIGN_SCREEN_HEIGHT) / 2.0f);

}

void GamePlayingStage1::updateControl()
{
	// character control
	switch(ControlButton::getInstance()->checkClick())
	{
	case BUTTON_LEFT:
		move(GAME_MOVE_LEFT);
		ControlButton::getInstance()->clear();
		break;
	case BUTTON_MIDDLE:
		move(GAME_MOVE_MID);
		ControlButton::getInstance()->clear();
		break;
	case BUTTON_RIGHT:
		move(GAME_MOVE_RIGHT);
		ControlButton::getInstance()->clear();
		break;
	}

	// pause control
	if(mSceneManager->mMenus[RES_PAUSE_BUTTON_ID]->isButtonPress(0))
	{
		mPopupMenu->show(POPUP_PAUSE);
	}
}

void GamePlayingStage1::updateMoving()
{
	if(hasFlag(GAME_PLAYING_JUMP_UP))
	{
		return;
	}
	if(hasFlag(GAME_PLAYING_JUMP_END))
	{
		MapBox* box = mLevelInfo.mMapObject->getCurBox();
		if(box->isVibrated())
		{
			removeFlag(GAME_PLAYING_MOVING);
			removeFlag(GAME_PLAYING_JUMP_END);
			checkNewState();
		}
		else
		{
			updateCharacter();
		}
	}
	else
	{
		transTo(0, 0, false);
	}
}

void GamePlayingStage1::updateCharacter()
{
	MapBox* obj = mLevelInfo.mMapObject->getCurBox();
	mCharacter->mPostion.y = obj->mPostion.y + CHARACTER_DELTA_Y;
}


void GamePlayingStage1::updatePathDisplay()
{
	if(timerStarted())
	{
		mIsWaitForDisplay = !waitForTimeout();
		if(!mIsWaitForDisplay)
		{
			checkNewState();
		}
	}
	else
	{
		setTimeout(HIGHLIGHT_AT_GAMEOVER_TIMEOUT);
		startTimer();
		mLevelInfo.mMapObject->setState(MAP_HIGHLIGHT_PATH);
	}
}

void GamePlayingStage1::update()
{
	StageBase::update();
	// only update when resource has been loaded
	if(!mLoaded)
	{
		return;
	}

	if(mIsWaitForDisplay)
	{
		checkNewState();
		return;
	}

	// when a popup has been shown
	if(mPopupMenu->opened())
	{
		mPopupMenu->update();
		mCharacter->setVisible(false);
		if(mHasShowHelp)
		{
			if(mPopupMenu->isResume(POPUP_GUIDE1))
			{
				mPopupMenu->close();
				mCharacter->setVisible(true);
			}			
		}
		else
		{
			checkNewState();
		}
		return;// only popup has been update, other will be disabled
	}
	else
	{
		removeFlag(GAME_PLAYING_POPUP);
		if(mHasShowHelp)
		{
			setLevel(1);
			mHasShowHelp = false;
		}
	}

	// enable character control when a move step is right
	// when move wrong, gamer will not control character
	if(mState == GAME_PLAYING_MOVE_OK)
	{
		updateControl();
	}

	// wait for timeout to start play at the begin level
	else if(mState == GAME_PLAYING_SHOWPATH)
	{
		showPath(false);
	}

	// when the character move from old position to new position,
	// it will be updated new coords to create a jump effect
	if(hasFlag(GAME_PLAYING_MOVING))
	{
		updateMoving();
	}

	mLevelInfo.mMapObject->update();
}

void GamePlayingStage1::render()
{
	if(mLoaded)
	{
		if(!mHasShowHelp)
		{
			mLevelInfo.mMapObject->render();
		}
		StageBase::render();
		mDrawer->render();
	}
}

void GamePlayingStage1::pause()
{
	mPopupMenu->show(POPUP_PAUSE);
}

void GamePlayingStage1::destroy()
{
	StageInfo::destroyInstance();
	SAFE_FREE(mPopupMenu);
	StageBase::destroy();
	PlayingStageDrawer::destroyInstance();
}

int GamePlayingStage1::getState() const
{
	return mState;
}

GamePlayingStage1::GamePlayingStage1()
{
	mShowPathTimeout = AMOUNT_TIME_OUT;
	printf("In GamePlayingStage1 - AMOUNT_TIME_OUT is : %d\n", AMOUNT_TIME_OUT);
	mStageInfo = NULL;
	mLevelInfo.mMapObject = NULL;
	mStartTime = 0;
	mState = 0;
	mIsReady = false;
	mStageId = 1;
	mHasShowHelp = false;
	mIsWaitForDisplay = false;
	mIsShowPathAtGameOver = false;

	mDrawer = PlayingStageDrawer::getInstance();
}

GamePlayingStage1::~GamePlayingStage1()
{
	destroy();
}