#pragma once
#include "StageManager.h"

class PlayingStageDrawer
{
private:
	static PlayingStageDrawer* mInstance;
	PlayingStageDrawer(){}
	PlayingStageDrawer(const PlayingStageDrawer &){} 
	PlayingStageDrawer& operator =(const PlayingStageDrawer &){}
	~PlayingStageDrawer(void);
public:
	static PlayingStageDrawer* getInstance();
	static void destroyInstance();
	void setLevel(int level);
	void setPlayState(GameState state);
	void render();
	void update();
	void destroy();
};
