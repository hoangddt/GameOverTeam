#pragma once
#include "StageBase.h"

/*
This define depend on MainMenu/SM.txt
 */
#define MAIN_MENU_ID	0
#define MENU_SETTING_ID 1
#define MENU_CREDIT_ID	2
#define MENU_EXIT_ID    3


class GameMainMenuStage: public StageBase
{
private:
	Control2D* mSettingMenuItem;
	Control2D* mCreditMenuItem;
	Control2D* mMainMenu;
	Control2D* mExitPopUpMenu;
	bool mSettingSoundTurnOn;
	void updateSoundButton();
	void updateSettingPopup();
	void updateCreditPopup();
public:
	virtual void start();
	virtual void update();
	bool isPlayPress() const;
	bool isExitPress() const;
};