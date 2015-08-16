#pragma once
#include "GamePlayingStage1.h"

class GamePlayingStage2: public GamePlayingStage1
{
private:
	bool mChecked;
protected:
	virtual void move(Direction direction);
	virtual void checkNewState();
public:
	virtual void setLevel(int level);
	virtual void update();
	GamePlayingStage2();
};
