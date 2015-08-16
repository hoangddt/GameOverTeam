#pragma once
#include "Control2D.h"

enum PopUpList
{
	POPUP_NONE,
	POPUP_PAUSE,
	POPUP_GAMEOVER,
	POPUP_WINSTAGE,
	POPUP_LEVELUP
};

class PopUpMenu
{
protected:
	Control2D* mMenus[4];
	int mTimeout;
	long mStartTime;
	bool mClosed;
	bool mOpened;
public:
	PopUpMenu(Control2D* pause,
			  Control2D* gameover,
			  Control2D* winstage,
			  Control2D* levelup);
	~PopUpMenu();
	bool closed() const;
	bool opened() const;
	void close();
	void update();
	void show(PopUpList, int timeout = -1);
	bool isMainMenu(PopUpList);
	bool isTryAgain(PopUpList);
	bool isResume(PopUpList);
	bool isNextLevel(PopUpList);
};