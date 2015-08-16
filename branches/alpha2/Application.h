#pragma once
#include "StageManager.h"

class Application
{
private:
	static Application* mpInstance;
	StageManager* mStageManager;
	Application(){}
	Application(const Application &){} 
	Application& operator =(const Application &){}
	~Application(void);
	unsigned int mScreenWidth;
	unsigned int mScreenHeight;
public:
	static Application* getInstance();
	static void destroyInstance();
	static void call_exit();
	void init(unsigned int screenWidth, unsigned int screenHeight);
	void render();
	void update();
	void destroy();
	unsigned int getScreenWidth();
	unsigned int getScreenHeight();
};
