#pragma once
#include "StageBase.h"

#define LOGO_TIMEOUT 1500

class GameLogoStage: public StageBase
{
private:
	bool mCompleted;
	long mStartTime;
public:
	virtual void update();
	virtual void start();
	bool completed() const;
	GameLogoStage();
};