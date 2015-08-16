#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"
#include "StageInfo.h"
#include "StageBase.h"

enum Direction
{
	GAME_MOVE_LEFT,
	GAME_MOVE_MID,
	GAME_MOVE_RIGHT
};

enum PlayingState
{
	GAME_PLAYING_SHOWPATH	= 0x01,// hien duong dan
	GAME_PLAYING_POPUPMENU	= 0x02,// hien popup menu
	GAME_PLAYING_MOVE_OK	= 0x04,// ng choi di 1 buoc dung
	GAME_PLAYING_MOVE_FAIL	= 0x08,// ng choi di 1 buoc sai
	GAME_PLAYING_MOVE_END	= 0x10,// ng choi ket thuc map hien tai
	GAME_PLAYING_MOVING		= 0x20,// nhan vat dang di chuyen
	GAME_PLAYING_WIN		= 0x40 // win o stage hien tai
};

class GamePlayingStage1: public StageBase
{
private:
	int mState;
	int mShowPathTimeout;

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;

	StageInfo* mStageInfo;
	LevelInfo mLevelInfo;

	GameObject* mCharacter;

	void move(Direction direction);
	void showPath(bool init);
	void showPopup();
	void setLevel(int level);
	void transTo(uint row, uint col, bool init);
	void checkLevelUp();
	bool hasFlag(uint flag);
	void addFlag(uint flag);
	void removeFlag(uint flag);
public:
	void play();
	void init(const char* rmFile, const char* smFile, const char* stageFile, int level);
	virtual void update();
	virtual void render();
	virtual void destroy();
	int getState() const;
	GamePlayingStage1();
	~GamePlayingStage1();
};
