#include "InputManager.h"
#include "../Utilities/Utilities.h"
#include "KeyBuffer.h"
#include "Lib.h"

InputManager* InputManager::mInstance = NULL;

void InputManager::onKeyEvent(int keyCode, bool isDown)
{
	if(mInstance != NULL)
	{
		KeyState state = isDown ? KEY_DOWN : KEY_UP;
		mInstance->mpKeyBuffer->setKeyState(keyCode, state);
	}
}
void InputManager::onMouseEvent(int type, int x, int y)
{
	if(mInstance == NULL)
	{
		return;
	}
	mInstance->mMouseState = (MouseState)type;
	mInstance->mOldMouseX = mInstance->mMouseX;
	mInstance->mOldMouseY = mInstance->mMouseY;
	mInstance->mMouseX = x;
	mInstance->mMouseY = y;
}

InputManager* InputManager::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new InputManager();
	}
	return mInstance;
}

void InputManager::destroyInstance()
{
	SAFE_FREE(mInstance);
}

bool InputManager::isKeyDown(int keyCode)
{
	return mpKeyBuffer->isKeyDown(keyCode);
}

bool InputManager::isKeyUp(int keyCode)
{
	return mpKeyBuffer->isKeyUp(keyCode);
}

bool InputManager::hasTouch(TouchEvent touchEvent)
{
	switch(touchEvent)
	{
	case TOUCH_DOWN:
		return mMouseState == MOUSE_DOWN;
	case TOUCH_UP:
		return mMouseState == MOUSE_UP;
	case TOUCH_DRAG:
		return mMouseState == MOUSE_DRAG;
	default:
		return false;
	}
}

TouchData InputManager::getTouchData()
{
	TouchData touchData;
	touchData.x = mMouseX;
	touchData.y = mMouseY;
	touchData.dentalX = mMouseX - mOldMouseX;
	touchData.dentalY = mMouseY - mOldMouseY;
	return touchData;
}

void InputManager::clearTouchState()
{
	mMouseState = MOUSE_NONE;
}

void InputManager::init()
{
	mMouseState = MOUSE_NONE;
	mMouseX = 0;
	mMouseY = 0;
	mOldMouseX = 0;
	mOldMouseY = 0;
	mpKeyBuffer = new KeyBuffer(255);
	RegisterKeyFunc(InputManager::onKeyEvent);
	RegisterMouseFunc(InputManager::onMouseEvent);
}

InputManager::~InputManager(void)
{
	SAFE_FREE(mpKeyBuffer);
}
