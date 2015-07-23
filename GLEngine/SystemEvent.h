#pragma once
#include "Button.h"
#include "InputManager.h"

class SystemEvent
{
private:
	Button** mButtons;
	int mCountOfButtons;
	Button* mLastBtnDown;
	bool mEnable;
public:
	void setEnable(bool enable);
	void loop();
	SystemEvent(Button** buttons, int count);
	~SystemEvent(void);
};
