#include "SystemEvent.h"

void SystemEvent::loop()
{
	if(!mEnable)
	{
		return;
	}
	InputManager* inMgr = InputManager::getInstance();
	if (inMgr->hasTouch(TOUCH_DOWN))
	{
		if(mLastBtnDown != NULL)
		{
			mLastBtnDown->fireOnTouchEvent(TOUCH_NONE);
			mLastBtnDown = NULL;
		}
		TouchData touchData = inMgr->getTouchData();
		for (unsigned int i = 0; i < mCountOfButtons; i++)
		{
			Button* button = mButtons[i];
			if (button->isVisible() && button->containCoord(touchData.x, touchData.y))
			{
				button->fireOnTouchEvent(TOUCH_DOWN);
				mLastBtnDown = button;
				break;
			}
		}
	}
	if(inMgr->hasTouch(TOUCH_UP))
	{
		TouchData touchData = inMgr->getTouchData();
		for (unsigned int i = 0; i < mCountOfButtons; i++)
		{
			Button* button = mButtons[i];
			if (button->isVisible() && button->containCoord(touchData.x, touchData.y))
			{
				TouchEvent touchEvent = (button == mLastBtnDown) ? TOUCH_UP : TOUCH_NONE;
				mLastBtnDown->fireOnTouchEvent(touchEvent);
				mLastBtnDown = NULL;
				break;
			}
		}
	}
}

void SystemEvent::setEnable(bool enable)
{
	mEnable = enable;
}

SystemEvent::SystemEvent(Button** buttons, int count)
{
	mButtons = buttons;
	mCountOfButtons = count;
	mEnable = true;
	mLastBtnDown = NULL;
}

SystemEvent::~SystemEvent(void)
{
}