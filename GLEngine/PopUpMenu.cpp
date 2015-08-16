#include "PopUpMenu.h"
#include "Lib.h"

PopUpMenu::PopUpMenu(Control2D* pause,
					 Control2D* gameover,
					 Control2D* winstage,
					 Control2D* levelup,
					 Control2D* guide1,
					 Control2D* guide2,
					 Control2D* guide3)
{
	mMenus[POPUP_PAUSE - 1] = pause;
	mMenus[POPUP_GAMEOVER - 1] = gameover;
	mMenus[POPUP_WINSTAGE - 1] = winstage;
	mMenus[POPUP_LEVELUP - 1] = levelup;
	mMenus[POPUP_GUIDE1 - 1] = guide1;
	mMenus[POPUP_GUIDE2 - 1] = guide2;
	mMenus[POPUP_GUIDE3 - 1] = guide3;
	mClosed = true;
	mOpened = false;
	mCurrentPopup = POPUP_NONE;
}

PopUpMenu::~PopUpMenu()
{
}

bool PopUpMenu::closed() const
{
	return mClosed;
}

bool PopUpMenu::opened() const
{
	return mOpened;
}

void PopUpMenu::close()
{
	mOpened = false;
	mClosed = true;
	show(POPUP_NONE);
}

void PopUpMenu::update()
{
	if(mClosed || !mOpened)
	{
		show(POPUP_NONE);
	}
	else if(mTimeout > 0)
	{
		clock_t now = clock();
		now *= 1000.0f / CLOCKS_PER_SEC;
		if(now - mStartTime > mTimeout)
		{
			close();
		}
	}
}

void PopUpMenu::show(PopUpList element, int timeout)
{
	clock_t now = clock();
	now *= 1000.0f / CLOCKS_PER_SEC;
	mStartTime = now;

	mTimeout = timeout;
	for(int i = 0; i < 7; i++)
	{
		mMenus[i]->setVisible(COLLAPSED);
	}
	if(element != POPUP_NONE)
	{
		printf("show %d\n", element);
		mMenus[element - 1]->setVisible(VISIBILITY);
		mOpened = true;
		mClosed = false;
	}
	else
	{
		mOpened = false;
		mClosed = true;
	}

	mCurrentPopup = element;
}

bool PopUpMenu::isMainMenu(PopUpList element)
{
	bool value = false;
	switch (element)
	{
	case POPUP_PAUSE:
		if (mMenus[element - 1]->isButtonPress(3))
		{
			value = true;
		}
		break;
	case POPUP_GAMEOVER:
		if (mMenus[element - 1]->isButtonPress(2))
		{
			value = true;
		}
		break;
	case POPUP_WINSTAGE:
		if (mMenus[element - 1]->isButtonPress(3))
		{
			value = true;
		}
		break;
	}
	return value;
}

bool PopUpMenu::isTryAgain(PopUpList element)
{
	bool value = false;
	switch (element)
	{
	case POPUP_PAUSE:
		if (mMenus[element - 1]->isButtonPress(2))
		{
			value = true;
		}
		break;
	case POPUP_GAMEOVER:
		if (mMenus[element - 1]->isButtonPress(1))
		{
			value = true;
		}
		break;
	case POPUP_WINSTAGE:
		if (mMenus[element - 1]->isButtonPress(2))
		{
			value = true;
		}
		break;
	}
	return value;
}

bool PopUpMenu::isResume(PopUpList element)
{
	switch (element)
	{
	case POPUP_PAUSE:
		return mMenus[element - 1]->isButtonPress(1);
	case POPUP_GUIDE1:
	case POPUP_GUIDE2:
	case POPUP_GUIDE3:
		return mMenus[element - 1]->isButtonPress(1);
	}
	return false;
}

bool PopUpMenu::isNextLevel(PopUpList element)
{
	bool value = false;
	switch (element)
	{
	case POPUP_WINSTAGE:
		if (mMenus[element - 1]->isButtonPress(3))
		{
			value = true;
		}
		break;
	}
	return value;
}