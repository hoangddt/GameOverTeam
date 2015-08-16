#pragma once
#include "StageBase.h"

class GameWinStage: public StageBase
{
public:
	virtual void update();
	GameWinStage(void);
	~GameWinStage(void);
};
