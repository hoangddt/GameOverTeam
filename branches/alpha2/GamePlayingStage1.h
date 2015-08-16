#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"
#include "StageInfo.h"
#include "StageBase.h"
#include "ArrowObject.h"
#include "PopUpMenu.h"

#define RES_CHARACTER_ID	1
#define AMOUNT_TIME_OUT		2500
#define CHARACTER_DELTA_Y	1.4f

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
	GAME_PLAYING_POPUP		= 0x100 // dang hien popup
};

class GamePlayingStage1: public StageBase
{
protected:
	int mState;
	int mShowPathTimeout;

	PopUpMenu* mPopupMenu;

	StageInfo* mStageInfo;
	LevelInfo mLevelInfo;
	int mStartTime;
	GameObject* mCharacter;
	bool mIsReady;

	virtual void move(Direction direction);
	void showPath(bool init);
	void transTo(uint row, uint col, bool init);
	void ready();
	virtual void checkNewState();
	void addFlag(uint flag);
	void removeFlag(uint flag);

	void updateControl();
	void updateMoving();
	void updateCharacter();
public:
	virtual void setLevel(int level);
	int getLevel() const;
	int getCountOfLevels() const;
	void nextLevel();
	bool hasFlag(uint flag);
	virtual void start();
	virtual void init(const char* rmFile, 
					  const char* smFile, 
					  const char* stageFile,
					  const char* bgSoundId);
	virtual void update();
	virtual void render();
	virtual void destroy();
	int getState() const;
	GamePlayingStage1();
	virtual ~GamePlayingStage1();
};
