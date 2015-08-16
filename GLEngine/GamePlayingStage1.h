#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"
#include "StageInfo.h"
#include "StageBase.h"
#include "ArrowObject.h"
#include "PopUpMenu.h"

#define RES_CHARACTER_ID				1
#define RES_LEVELINFO_ID				9
#define AMOUNT_TIME_OUT					2500
#define HIGHLIGHT_AT_GAMEOVER_TIMEOUT	1500
#define CHARACTER_DELTA_Y				1.4f
#define RES_PAUSE_BUTTON_ID				4
#define RES_GUIDE1_ID					5
#define RES_GUIDE2_ID					6
#define RES_GUIDE3_ID					7

enum Direction
{
	GAME_MOVE_LEFT,
	GAME_MOVE_MID,
	GAME_MOVE_RIGHT
};

enum PlayingState
{
	GAME_PLAYING_SHOWPATH	= 0x001,// hien duong dan
	GAME_PLAYING_MOVE_OK	= 0x002,// ng choi di 1 buoc dung
	GAME_PLAYING_MOVE_FAIL	= 0x004,// ng choi di 1 buoc sai
	GAME_PLAYING_MOVE_END	= 0x008,// ng choi ket thuc map hien tai
	GAME_PLAYING_MOVING		= 0x010,// nhan vat dang di chuyen
	GAME_PLAYING_GOTO_MAIN	= 0x020,// quay tro ve main menu
	GAME_PLAYING_TRY_AGAIN	= 0x040,// choi lai
	GAME_PLAYING_JUMP_END	= 0x080,// khi nhan vat jump end -> check new state
	GAME_PLAYING_POPUP		= 0x100,// dang hien popup
	GAME_PLAYING_JUMP_UP	= 0x200 // nhay len cao tai 1 cho
};

class PlayingStageDrawer;

class GamePlayingStage1: public StageBase
{
protected:
	int mStageId;
	int mState;
	int mShowPathTimeout;
	bool mHasShowHelp;
	bool mIsWaitForDisplay;
	bool mIsShowPathAtGameOver;

	PopUpMenu* mPopupMenu;
	PlayingStageDrawer* mDrawer;

	StageInfo* mStageInfo;
	LevelInfo mLevelInfo;
	int mStartTime;
	GameObject* mCharacter;
	bool mIsReady;

	virtual void move(Direction direction);
	void showPath(bool init);
	void transTo(uint row, uint col, bool init);
	virtual void ready();
	virtual void checkNewState();
	void addFlag(uint flag);
	void removeFlag(uint flag);

	void updateControl();
	void updateMoving();
	void updateCharacter();
	void updatePathDisplay();
public:
	virtual void setLevel(int level);
	int getLevel() const;
	int getCountOfLevels() const;
	void nextLevel();
	bool hasFlag(uint flag);
	virtual void start();
	virtual void init(const char* rmFile, 
					  const char* smFile, 
					  const char*   ,
					  const char* bgSoundId);
	virtual void update();
	virtual void render();
	void pause();
	virtual void destroy();
	int getState() const;
	GamePlayingStage1();
	virtual ~GamePlayingStage1();
};
