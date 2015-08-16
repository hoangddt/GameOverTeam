#pragma once
#include "GamePlayingStage1.h"
#include "GamePlayingStage2.h"
#include "LoadingScreen.h"

enum Stage
{
	STAGE_LOGO,
	STAGE_LOADING,
	STAGE_MAINMENU,
	STAGE_MAP,
	STAGE_EXIT,
	STAGE_PLAYING1,
	STAGE_PLAYING2,
	STAGE_PLAYING3,
	STAGE_ENDING
};

enum GameState
{
	GAME_PLAYING,
	GAME_LEVEL_UP,
	GAME_OVER,
	GAME_WIN_STAGE,
	GAME_WIN
};

class PlayingStageDrawer;

class StageManager
{
private:
	static StageManager* mInstance;
	Stage mMaxPlayingStage;
	Stage mStage;
	StageBase* mStageInstace;
	LoadingScreen* mLoadingScreen;
	bool mLoading;

	void logoLogic();
	void mapLogic();
	void playLogic();
	void menuLogic();

	StageManager();
	StageManager(const StageManager &){}
	StageManager& operator =(const StageManager &){}
	~StageManager(void);
public:
	static StageManager* getInstance();
	static void destroyInstance();
	void setStage(Stage stage);
	void render();
	void update();
	void destroy();
};
