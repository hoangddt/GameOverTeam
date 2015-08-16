#pragma once
#include "Button.h"
#include "Camera.h"
#include "InputManager.h"
#define ALPHA_MIDDLE 3.141592653589 / 9

enum ButtonState {
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	BUTTON_NONE
};

class ControlButton
{
private:
	ControlButton(){}
	~ControlButton();
	ControlButton(const ControlButton&) {}
	ControlButton& operator= (const ControlButton&) {}
	static ControlButton* mInstance;
public:
	
	static ControlButton* getInstance();
	static void destroyInstance();

	// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE
	ButtonState checkClick();
};