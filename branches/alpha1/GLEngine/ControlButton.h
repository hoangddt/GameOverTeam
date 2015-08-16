#pragma once
#include "Button.h"
#include "Camera.h"
#include "InputManager.h"


enum ButtonState {
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	BUTTON_NONE
};

class ControlButton
{
private:
	ControlButton();
	~ControlButton();
	ControlButton(const ControlButton&) {}
	ControlButton& operator= (const ControlButton&) {}
	Button** mButtons;
	int mCountOfButtons;
	static ControlButton* mInstance;
public:
	
	static ControlButton* getInstance();
	static void destroyInstance();

	void init();
	void update();
	void render(Camera* camera);
	void destroy();
	void setPosition(float x, float y, float z);
	void setScaling(float x, float y, float z);
	// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE
	ButtonState checkClick();
};