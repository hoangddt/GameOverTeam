#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"

enum Direction
{
	MOVE_LEFT,
	MOVE_MID,
	MOVE_RIGHT
};

enum PlayingState
{
	GAME_PLAYING_SHOWPATH	= 0x01,// hien duong dan
	GAME_PLAYING_POPUPMENU	= 0x02,// hien popup menu
	GAME_PLAYING_MOVE_OK	= 0x04,// ng choi di 1 buoc dung
	GAME_PLAYING_MOVE_FAIL	= 0x08,// ng choi di 1 buoc sai
	GAME_PLAYING_MOVE_END	= 0x0F // ng choi ket thuc map hien tai
};

class GamePlayingState1: ITouchListener
{
private:
	PlayingState mState;
	int mShowPathTimeout;

	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;

	uint mLevel;
	Vector2** mLevelTable;
	uint mCountOfLevels;

	GameObject* mMapBox;

	void move(Direction direction);
	void showPath(bool init);
	void showPopup();
	void setLevel(int level);
public:
	static GamePlayingState1* getInstance();
	static void destroyInstance();
	virtual void touchPerformed(int buttonId);
	void play();
	void init(const char* rmFile, const char* smFile, int level);
	void update();
	void render();
	void destroy();
	PlayingState getState() const;
	GamePlayingState1(Vector2** levelTable, int countOfLevels);
	~GamePlayingState1();
};
