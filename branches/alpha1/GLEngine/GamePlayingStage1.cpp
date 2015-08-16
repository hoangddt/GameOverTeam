#include "GamePlayingStage1.h"
#include "Map.h"
#include "ControlButton.h"
#include <math.h>

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

	if(mapObj->isValidCurPosition())
	{
		addFlag(GAME_PLAYING_MOVE_OK);
		removeFlag(GAME_PLAYING_MOVE_FAIL);
	}
	else
	{
		
		addFlag(GAME_PLAYING_MOVE_FAIL);
		removeFlag(GAME_PLAYING_MOVE_OK);
	}

	curPos = mapObj->getCurrentPosition();
	if(curPos.x == 0)
	{
		addFlag(GAME_PLAYING_MOVE_END);
	}
}

void GamePlayingStage1::showPath(bool init)
{
	static int count = 0;
	MapObject* mapObj = mLevelInfo.mMapObject;

	if(init)
	{
		count = 0;
		mState = GAME_PLAYING_SHOWPATH;
		mapObj->setState(MAP_SHOW_PATH);
	}

	if(count >= mShowPathTimeout && mShowPathTimeout > 0)
	{
		mapObj->setState(MAP_NORMAL);
		count = -1;

		if(mShowPathTimeout > 0)
		{
			play();
		}
	}
	else if(count > -1)
	{
		count += 10;
	}
}

void GamePlayingStage1::showPopup()
{
}

void GamePlayingStage1::play()
{
	mState = GAME_PLAYING_MOVE_OK;
	mLevelInfo.mMapObject->startGame();
	GameObject* obj = mLevelInfo.mMapObject->getCurBox();

	mCharacter->mPostion.x = obj->mPostion.x + obj->getActualWidth() / 2.0f;
	mCharacter->mPostion.z = obj->mPostion.z + obj->getActualHeight() / 2.0f;
	mCharacter->mPostion.y = obj->mPostion.y;
	mCharacter->setVisible(true);
}

void GamePlayingStage1::setLevel(int level)
{
	mLevelInfo = mStageInfo->getLevelInfo(level);
	showPath(true);
}

void GamePlayingStage1::checkLevelUp()
{
	if(hasFlag(GAME_PLAYING_MOVE_OK) && hasFlag(GAME_PLAYING_MOVE_END))
	{
		mCharacter->setVisible(false);
		if(mLevelInfo.mLevel < mStageInfo->getCountOfLevels())
		{
			MessageBoxA(NULL, "Level up", "GAME", 0);
			setLevel(mLevelInfo.mLevel + 1);
		}
		else
		{
			addFlag(GAME_PLAYING_WIN);
			MessageBoxA(NULL, "Win this stage", "GAME", 0);
		}
	}
	else if(hasFlag(GAME_PLAYING_MOVE_FAIL))
	{
		mCharacter->setVisible(false);
		MessageBoxA(NULL, "Gameover!", "GAME", 0);
	}
}

void GamePlayingStage1::transTo(uint row, uint col, bool init)
{
	static float vy;
	static float vx;
	static float vz;
	static float time;
	static float alpha = DEGREES_TO_RADIANS(45);
	static float g = 9.8f;

	static float startX;
	static float startY;
	static float startZ;

	static float signX;
	static float signZ;

	static float deltaZ;
	static float deltaX;

	if(init)
	{
		MapObject* mapObj = mLevelInfo.mMapObject;
		GameObject* obj = mapObj->getCurBox();
		Vector2 cPos = mapObj->getCurrentPosition();

		deltaZ = (row - cPos.x) * obj->getActualHeight();
		deltaX = (col - cPos.y) * obj->getActualWidth();

		signZ = row > cPos.x ? 1 : (row < cPos.x ? -1 : 0);
		signX = col > cPos.y ? 1 : (col < cPos.y ? -1 : 0);

		float lmax = sqrt(deltaZ * deltaZ + deltaX * deltaX );
		vy = sqrt((g * lmax) / sin(2.0f * alpha));
		vx = sqrt(fabs(g * deltaX) / sin(2.0f * alpha));
		vz = sqrt(fabs(g * deltaZ) / sin(2.0f * alpha));

		time = 0.0f;

		startX = mCharacter->mPostion.x;
		startY = mCharacter->mPostion.y;
		startZ = mCharacter->mPostion.z;

		mCharacter->mPostion.y = 0.005f;

		addFlag(GAME_PLAYING_MOVING);
	}

	time += 0.03f;

	bool complete = true;

	if(mCharacter->mPostion.y <= 0.001f)
	{
		mCharacter->mPostion.y = 0;
	}
	else
	{
		mCharacter->mPostion.y = startY + vy * time * sin(alpha) - 0.5 * g * time * time;
		complete = false;
	}

	if(fabs(mCharacter->mPostion.x - (startX + deltaX)) < 0.03f)
	{
		mCharacter->mPostion.x = startX + deltaX;
	}
	else
	{
		mCharacter->mPostion.x = startX + signX * vx * time * cos(alpha);
		complete = false;
	}

	if(fabs(mCharacter->mPostion.z - (startZ + deltaZ)) < 0.03f)
	{
		mCharacter->mPostion.z = startZ + deltaZ;
	}
	else
	{
		mCharacter->mPostion.z = startZ + signZ * vz * time * cos(alpha);
		complete = false;
	}
	
	if(complete)
	{
		removeFlag(GAME_PLAYING_MOVING);
		checkLevelUp();
	}
}

void GamePlayingStage1::init(const char* rmFile, const char* smFile, const char* stageFile, int level)
{
	ResourceManager::destroyInstance();
	mResourceManager = ResourceManager::getInstance();
	mResourceManager->init(rmFile);

	SceneManager::destroyInstance();
	mSceneManager = SceneManager::getInstance();
	mSceneManager->init(smFile);

	StageInfo::destroyInstance();
	mStageInfo = StageInfo::getInstance();
	mStageInfo->init(stageFile);

	//mSceneManager->mObjects[1]->setTexture(mStageInfo->getWallTexture);
	//mSceneManager->mObjects[2]->setTexture(mStageInfo->getDirtyTexture);
	mCharacter = mSceneManager->mObjects[0];// mat dinh nhan vat id 0

	setLevel(level);
}

void GamePlayingStage1::update()
{

	if(mState == GAME_PLAYING_MOVE_OK)
	{
		switch(ControlButton::getInstance()->checkClick())
		{
		case BUTTON_LEFT:
			move(GAME_MOVE_LEFT);
			break;
		case BUTTON_MIDDLE:
			move(GAME_MOVE_MID);
			break;
		case BUTTON_RIGHT:
			move(GAME_MOVE_RIGHT);
			break;
		}
	}

	if(hasFlag(GAME_PLAYING_SHOWPATH))
	{
		showPath(false);
	}
	if(hasFlag(GAME_PLAYING_POPUPMENU))
	{
	}
	if(hasFlag(GAME_PLAYING_MOVE_OK))
	{
	}
	if(hasFlag(GAME_PLAYING_MOVE_FAIL))
	{
	}
	if(hasFlag(GAME_PLAYING_MOVE_END))
	{
	}
	if(hasFlag(GAME_PLAYING_MOVING))
	{
		transTo(0, 0, false);
	}

	mSceneManager->update();
	mLevelInfo.mMapObject->update();
}

void GamePlayingStage1::render()
{
	mSceneManager->render();
	mLevelInfo.mMapObject->render();
}

void GamePlayingStage1::destroy()
{
	StageInfo::destroyInstance();
	SceneManager::destroyInstance();
	ResourceManager::destroyInstance();
	ControlButton::destroyInstance();
}

int GamePlayingStage1::getState() const
{
	return mState;
}

GamePlayingStage1::GamePlayingStage1()
{
	mShowPathTimeout = 1000;
	mStageInfo = NULL;
	mLevelInfo.mMapObject = NULL;
}

GamePlayingStage1::~GamePlayingStage1(void)
{
	destroy();
}
