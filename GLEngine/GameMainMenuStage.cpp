#include "GameMainMenuStage.h"
#include "LogHeaderAndroid.h"
#include "SoundManager.h"
#include "Application.h"
#include "GameData.h"

enum Main_Menu_Button
{
	BUTTON_PLAY_PRESSED = 1,
	BUTTON_SETTING_PRESSED = 2,
	BUTTON_CREDIT_PRESSED = 3,
	BUTTON_EXIT_PRESSED = 4
};

bool GameMainMenuStage::isPlayPress() const
{
	if(!mLoaded)
	{
		return false;
	}
	return mMainMenu->isButtonPress(BUTTON_PLAY_PRESSED);
}

bool GameMainMenuStage::isExitPress() const
{
	const int EXIT_YES = 1,
			  EXIT_NO  = 2;

	if (mExitPopUpMenu->isButtonPress(EXIT_YES))
	{
		Application::call_exit();
		return true;
	}
	if (mExitPopUpMenu->isButtonPress(EXIT_NO))
	{
		mExitPopUpMenu->setVisible(COLLAPSED);
		mMainMenu->setEnable(true);
		return false;
	}
	return false;
}

void GameMainMenuStage::updateSoundButton()
{
	const int TEXTURE_ON_ID = 1,
			  TEXTURE_OFF_ID = 2;
	Texture* texture = mSettingSoundTurnOn ? 
		mResourceManager->getTextureById(TEXTURE_ON_ID) : 
	mResourceManager->getTextureById(TEXTURE_OFF_ID);
	mSettingMenuItem->setTexture(texture);
	GameData::getInstance()->setSoundSetting(mSettingSoundTurnOn ? 1 : 0);
}

void GameMainMenuStage::updateSettingPopup()
{
	enum Sound_Setting_Button
	{
		BUTTON_OFF_PRESSED = 1,
		BUTTON_ON_PRESSED  = 2,
		BUTTON_EXIT_PRESSED = 3
	};

	if(mSettingSoundTurnOn && mSettingMenuItem->isButtonPress(BUTTON_OFF_PRESSED))
	{
		// Press Tunrn Off Sound
		mSettingSoundTurnOn = !mSettingSoundTurnOn;
		updateSoundButton();
		SoundManager::getInstance()->setMute(true);
	}
	else if(!mSettingSoundTurnOn && mSettingMenuItem->isButtonPress(BUTTON_ON_PRESSED))
	{
		// Press Tunrn On Sound
		mSettingSoundTurnOn = !mSettingSoundTurnOn;
		updateSoundButton();
		SoundManager::getInstance()->setMute(false);
	}
	else if(mSettingMenuItem->isButtonPress(BUTTON_EXIT_PRESSED))
	{
		// Press Exit Sound
		mSettingMenuItem->setVisible(COLLAPSED);
	}
}

void GameMainMenuStage::updateCreditPopup()
{
	if(mCreditMenuItem->isButtonPress(1))
	{
		mCreditMenuItem->setVisible(COLLAPSED);
	}
}

void GameMainMenuStage::update()
{
	if(!mLoaded)
	{
		return;
	}
	if(mSettingMenuItem->getIsVisible())
	{
		updateSettingPopup();	
	}
	else if(mCreditMenuItem->getIsVisible())
	{
		updateCreditPopup();
	}
	else if(mExitPopUpMenu->getIsVisible())
	{
		isExitPress();
	}
	else if(mMainMenu->isButtonPress(BUTTON_SETTING_PRESSED))
	{
		updateSoundButton();
		mSettingMenuItem->setVisible(VISIBILITY);
		mMainMenu->setEnable(false);
	}
	else if(mMainMenu->isButtonPress(BUTTON_CREDIT_PRESSED))
	{
		mCreditMenuItem->setVisible(VISIBILITY);
		mMainMenu->setEnable(false);
	}
	else if (mMainMenu->isButtonPress(BUTTON_EXIT_PRESSED))
	{
		mExitPopUpMenu->setVisible(VISIBILITY);
		mMainMenu->setEnable(false);
	}
	else
	{
		mMainMenu->setEnable(true);
	}
}

void GameMainMenuStage::start()
{
	mSettingMenuItem = mSceneManager->mMenus[MENU_SETTING_ID];
	mCreditMenuItem = mSceneManager->mMenus[MENU_CREDIT_ID];
	mMainMenu = mSceneManager->mMenus[MAIN_MENU_ID];
	mExitPopUpMenu = mSceneManager->mMenus[MENU_EXIT_ID];
	
	// mMainMenu->setVisible(COLLAPSED);
	mSettingMenuItem->setVisible(COLLAPSED);
	mCreditMenuItem->setVisible(COLLAPSED);
	mExitPopUpMenu->setVisible(COLLAPSED);
	mSettingSoundTurnOn = !SoundManager::getInstance()->getMuteStatus();
}