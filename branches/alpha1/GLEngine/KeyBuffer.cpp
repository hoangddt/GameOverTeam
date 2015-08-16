#include "KeyBuffer.h"
#include <stddef.h>
#include "Lib.h"

void KeyBuffer::checkValidKey(int keyCode) const
{
	if(keyCode < 0 || keyCode >= mMaxSupported)
	{
		throw "Out of bound!";
	}
}

void KeyBuffer::setKeyState(int keyCode, KeyState state)
{
	checkValidKey(keyCode);
	mpKeys[keyCode] = (char)state;
}

bool KeyBuffer::isKeyDown(int keyCode) const
{
	checkValidKey(keyCode);
	return (KeyState)mpKeys[keyCode] == KEY_DOWN;
}

bool KeyBuffer::isKeyUp(int keyCode) const
{
	checkValidKey(keyCode);
	return (KeyState)mpKeys[keyCode] == KEY_UP;
}

void KeyBuffer::reset()
{
	for(int i = 0; i < mMaxSupported; i++)
	{
		mpKeys[i] = KEY_NONE;
	}
}

KeyBuffer::KeyBuffer(int maxSupported)
{
	this->mMaxSupported = maxSupported;
	mpKeys = new char[mMaxSupported];
	reset();
}

KeyBuffer::~KeyBuffer(void)
{
	FREE_1D_ARRAY(mpKeys);
}
