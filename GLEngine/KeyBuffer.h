#pragma once

enum KeyState
{
	KEY_DOWN,
	KEY_UP,
	KEY_NONE
};

class KeyBuffer
{
private:
	int mMaxSupported;
	char* mpKeys;
	void checkValidKey(int keyCode) const;
public:
	void setKeyState(int keyCode, KeyState state);
	bool isKeyDown(int keyCode) const;
	bool isKeyUp(int keyCode) const;
	void reset();
	KeyBuffer(int maxSupported = 255);
	~KeyBuffer(void);
};
