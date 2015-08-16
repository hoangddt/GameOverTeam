#include "ControlButton.h"
#include "SceneManager.h"
#include "Application.h"
#include "LogHeaderAndroid.h"
#include <math.h>

ControlButton* ControlButton::mInstance = NULL;

ControlButton* ControlButton::getInstance()
{
	if (mInstance == NULL)
		mInstance = new ControlButton;
	return mInstance;
}
void ControlButton::destroyInstance()
{
	SAFE_FREE(mInstance);
}

ControlButton::~ControlButton()
{
}

// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE
ButtonState ControlButton::checkClick()
{
	InputManager* input = InputManager::getInstance();
	static int checkDown = 0;
	static TouchData downData;
	TouchData upData;
	if ( input->hasTouch(TOUCH_DOWN) )
	{
		downData = input->getTouchData();
		checkDown = 1;
	}
	if ( input->hasTouch(TOUCH_UP) && checkDown)
	{
		checkDown = 0;
		upData = input->getTouchData();
		double alpha;
		double tanAlpha;
		if ( downData.y < upData.y ) return BUTTON_NONE;
		tanAlpha = (double) (upData.x - downData.x) / (upData.y - downData.y);
		alpha = atan(tanAlpha);
		if ( (alpha <= ALPHA_MIDDLE) && (alpha >= -ALPHA_MIDDLE) )
		{
			return BUTTON_MIDDLE;
		}
		if ( alpha > ALPHA_MIDDLE) 
		{
			return BUTTON_LEFT;
		}
		if ( alpha < -ALPHA_MIDDLE) 
		{
			return BUTTON_RIGHT;
		}
	
	}
	return BUTTON_NONE;
}