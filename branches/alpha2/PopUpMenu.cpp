#include "PopUpMenu.h"
#include "Lib.h"

PopUpMenu::PopUpMenu(Control2D* pause,
					 Control2D* gameover,
					 Control2D* winstage,
					 Control2D* levelup)
{
	mMenus[0] = pause;
	mMenus[1] = gameover;
	mMenus[2] = winstage;
	mMenus[3] = levelup;
	mClosed = true;
	mOpened = false;
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
	for(int i = 0; i < 4; i++)
	{
		mMenus[i]->setVisible(COLLAPSED);
	}
	if(element != POPUP_NONE)
	{
		mMenus[element - 1]->setVisible(VISIBILITY);
		mOpened = true;
		mClosed = false;
	}
	else
	{
		mOpened = false;
		mClosed = true;
	}
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
	bool value = false;
	switch (element)
	{
	case POPUP_PAUSE:
		if (mMenus[element - 1]->isButtonPress(1))
		{
			value = true;
		}
		break;
	}
	return value;
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