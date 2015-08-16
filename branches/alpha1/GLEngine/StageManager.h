#pragma once
#include "GamePlayingStage1.h"

enum Stage
{
	STAGE_LOADING,
	STAGE_MAINMENU,
	STAGE_PLAYING1,
	STAGE_PLAYING2,
	STAGE_PLAYING3
};

class StageManager
{
private:
	static StageManager* mInstance;

	Stage mStage;
	StageBase* mStageInstace;

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
