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

ControlButton::ControlButton()
{
	InputManager* input = InputManager::getInstance();
	input->clearTouchState();
	mTestClear = false;
	mState = BUTTON_NONE;
}

ControlButton::~ControlButton()
{
}

void ControlButton::clear()
{
	InputManager* input = InputManager::getInstance();
	input->clearTouchState();
	mTestClear = false;
	mState = BUTTON_NONE;
}

// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE, BUTTON_UP
ButtonState ControlButton::checkClick()
{
	InputManager* input = InputManager::getInstance();
	static int checkDown = 0;
	static TouchData downData;
	static TouchData upData;
	
	if ( input->hasTouch(TOUCH_DOWN) )
	{
		//printf("DOWN\n");
		downData = input->getTouchData();
		checkDown = 1;
		mState = BUTTON_NONE;
	}
	upData = input->getTouchData();
	double xDistance, yDistance, distance;
	
	// Check slide
	xDistance = upData.x - downData.x;
	yDistance = upData.y - downData.y;
	distance = sqrt( pow(xDistance, 2) + pow(yDistance, 2) );
	if ( distance >= MAX_HANDLE && checkDown )
	{
		//printf("SLIDE\n");
		checkDown = 0;
		double alpha;
		double tanAlpha;
		tanAlpha = (double) (upData.x - downData.x) / (upData.y - downData.y);
		alpha = atan(tanAlpha);

		if ( downData.y <= upData.y )
		{
			if ( alpha > 0 )
			{
				mState = BUTTON_RIGHT;
			}
			else
			{
				mState = BUTTON_LEFT;
			}
		}
		else
		{
			if ( (alpha <= ALPHA_MIDDLE) && (alpha >= -ALPHA_MIDDLE) )
			{
				mState = BUTTON_MIDDLE;
			}
			if ( alpha > ALPHA_MIDDLE ) 
			{
				mState = BUTTON_LEFT;
			}
			if ( alpha < -ALPHA_MIDDLE ) 
			{
				mState = BUTTON_RIGHT;
			}
		}
		
		if ( downData.y < upData.y - MAX_HANDLE / 5 )
		{
			//printf("NONE\n");
			mState = BUTTON_NONE;
		}
	}
	
	// Check Click
	if ( checkDown && input->hasTouch(TOUCH_UP) )
	{
		checkDown = 0;
		mState = BUTTON_UP;
	}
	return mState;
}