#pragma once
class KeyBuffer;

enum TouchEvent
{
	TOUCH_DOWN,
	TOUCH_UP,
	TOUCH_DRAG,
	TOUCH_NONE
};

enum MouseState
{
	MOUSE_DOWN = 0,
	MOUSE_UP   = 1,
	MOUSE_DRAG = 2,
	MOUSE_NONE = 3
};

struct TouchData
{
	int x;
	int y;
	int dentalX;
	int dentalY;
};

class InputManager
{
private:
	static InputManager* mInstance;
	KeyBuffer* mpKeyBuffer;
	MouseState mMouseState;
	int mMouseX;
	int mMouseY;
	int mOldMouseX;
	int mOldMouseY;
	static void onKeyEvent(int keyCode, bool isDown);
	static void onMouseEvent(int type, int x, int y);
	InputManager(void){}
	InputManager(const InputManager &){} 
	InputManager& operator =(const InputManager &){}
	~InputManager(void);
public:
	static InputManager* getInstance();
	static void destroyInstance();
	bool isKeyDown(int keyCode); //return true if the key is pressed
	bool isKeyUp(int keyCode);//return true if the key is released
	bool hasTouch(TouchEvent touchEvent);//touch down/touch up/touch drag (or mouse drag)
	TouchData getTouchData();//return touch data, x, y, delta x, delta y
	void clearTouchState();
	void init();
};
