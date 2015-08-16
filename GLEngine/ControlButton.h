#pragma once
#include "Button.h"
#include "Camera.h"
#include "InputManager.h"
#define ALPHA_MIDDLE (3.141592653589 / 6)
#define MAX_HANDLE 20

enum ButtonState {
	BUTTON_LEFT = 0,
	BUTTON_MIDDLE = 1,
	BUTTON_RIGHT = 2,
	BUTTON_NONE = 3,
	BUTTON_UP = 4
};

class ControlButton
{
private:
	bool mIsDown;
	float mDeltaX;
	float mDeltaY;

	ControlButton();
	~ControlButton();
	ControlButton(const ControlButton&) {}
	ControlButton& operator= (const ControlButton&) {}
	static ControlButton* mInstance;
public:
	ButtonState mState;
	static ControlButton* getInstance();
	static void destroyInstance();
	
	bool mTestClear;
	// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE
	ButtonState checkClick();
	void clear();
};