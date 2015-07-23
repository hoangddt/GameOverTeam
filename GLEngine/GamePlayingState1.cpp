#include "GamePlayingState1.h"
#include "Map.h"

void GamePlayingState1::move(Direction direction)
{
	Vector2* vecLevel = mLevelTable[mLevel - 1];// x - row; y - col
	MapObject* mapObj = SceneManager::getInstance()->mMapObject;
	Vector2 curPosition = mapObj->getCurrentPosition();
	
	bool validPos = mapObj->isValidCurPosition();
	if(curPosition.x <= 0 || !validPos)
	{
		return;
	}

	curPosition.x--;

	switch(direction)
	{
	case MOVE_LEFT:// left
		if(curPosition.y <= 0)
		{
			return;
		}
		mapObj->setCharacterPosition(curPosition.x, curPosition.y - 1);
		break;
	case MOVE_MID:// mid
		mapObj->setCharacterPosition(curPosition.x, curPosition.y);
		break;
	case MOVE_RIGHT:// right
		if(curPosition.y >= vecLevel->y - 1)
		{
			return;
		}
		mapObj->setCharacterPosition(curPosition.x, curPosition.y + 1);
		break;
	}

	mState = mapObj->isValidCurPosition() ? GAME_PLAYING_MOVE_OK : GAME_PLAYING_MOVE_FAIL;
}

void GamePlayingState1::touchPerformed(int buttonId)
{
	switch(buttonId)
	{
	case 0:// left
		move(MOVE_LEFT);
		break;
	case 1:// mid
		move(MOVE_MID);
		break;
	case 2:// right
		move(MOVE_RIGHT);
		break;
	}
}

void GamePlayingState1::showPath(bool init)
{
	static int count = 0;
	MapObject* mapObj = mSceneManager->mMapObject;

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

void GamePlayingState1::showPopup()
{
	Button** buttons = mSceneManager->mButtons;
	int count = mSceneManager->mCountOfButtons;
}

void GamePlayingState1::play()
{
	mState = GAME_PLAYING_MOVE_OK;
	mSceneManager->mMapObject->startGame();
}

void GamePlayingState1::setLevel(int level)
{
	Vector2* vecLevel = mLevelTable[level - 1];
	mLevel = level;

	int** map = Map::getInstance()->genMap(vecLevel->x, vecLevel->y);
	SceneManager::getInstance()->genMapObject(map, vecLevel->x, vecLevel->y, mMapBox);
	FREE_2D_ARRAY(map, vecLevel->x);

	showPath(true);
}

void GamePlayingState1::init(const char* rmFile, const char* smFile, int level)
{
	ResourceManager::destroyInstance();
	mResourceManager = ResourceManager::getInstance();
	mResourceManager->init(rmFile);

	SceneManager::destroyInstance();
	mSceneManager = SceneManager::getInstance();
	mSceneManager->init(smFile);

	mMapBox = mSceneManager->mObjects[0];
	
	setLevel(level);
}

void GamePlayingState1::update()
{

	if(mState == GAME_PLAYING_MOVE_OK){
		static int a = 0;
		if(InputManager::getInstance()->isKeyDown('1'))
			a = 1;
		if(InputManager::getInstance()->isKeyUp('1') && a == 1){
			a = 0;
			touchPerformed(0);
		}

		static int b = 0;
		if(InputManager::getInstance()->isKeyDown('2'))
			b = 1;
		if(InputManager::getInstance()->isKeyUp('2') && b == 1){
			b = 0;
			touchPerformed(1);
		}

		static int c = 0;
		if(InputManager::getInstance()->isKeyDown('3'))
			c = 1;
		if(InputManager::getInstance()->isKeyUp('3') && c == 1){
			c = 0;
			touchPerformed(2);
		}
	}

	switch(mState)
	{
	case GAME_PLAYING_SHOWPATH:
		showPath(false);
		break;
	case GAME_PLAYING_POPUPMENU:
		showPopup();
		break;
	case GAME_PLAYING_MOVE_OK:
		break;
	case GAME_PLAYING_MOVE_FAIL:
		break;
	case GAME_PLAYING_MOVE_END:
		break;
	}
	mSceneManager->update();
}

void GamePlayingState1::render()
{
	mSceneManager->render();
}

void GamePlayingState1::destroy()
{
	mSceneManager->destroyInstance();
}

PlayingState GamePlayingState1::getState() const
{
	return mState;
}

GamePlayingState1::GamePlayingState1(Vector2** levelTable, int countOfLevels)
{
	mLevel = 1;
	mLevelTable = levelTable;
	mCountOfLevels = countOfLevels;
	mShowPathTimeout = 1000;
}

GamePlayingState1::~GamePlayingState1(void)
{
	destroy();
}
