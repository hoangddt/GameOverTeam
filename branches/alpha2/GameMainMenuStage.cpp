#include "GameMainMenuStage.h"

bool GameMainMenuStage::isPlayPress() const
{
	if(!mLoaded)
	{
		return false;
	}
	return mMainMenu->isButtonPress(1);
}

bool GameMainMenuStage::isExitPress() const
{
	if(!mLoaded)
	{
		return false;
	}
	return mMainMenu->isButtonPress(4);
}

void GameMainMenuStage::updateSoundButton()
{
	Texture* texture = mSettingSoundTurnOn ? 
		mResourceManager->getTextureById(1) : 
	mResourceManager->getTextureById(2);
	mSettingMenuItem->setTexture(texture);
}

void GameMainMenuStage::updateSettingPopup()
{
	if(mSettingSoundTurnOn && mSettingMenuItem->isButtonPress(1))
	{
		mSettingSoundTurnOn = !mSettingSoundTurnOn;
		updateSoundButton();
	}
	else if(!mSettingSoundTurnOn && mSettingMenuItem->isButtonPress(2))
	{
		mSettingSoundTurnOn = !mSettingSoundTurnOn;
		updateSoundButton();
	}
	else if(mSettingMenuItem->isButtonPress(3))
	{
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
	else if(mMainMenu->isButtonPress(2))
	{
		updateSoundButton();
		mSettingMenuItem->setVisible(VISIBILITY);
		mMainMenu->setEnable(false);
	}
	else if(mMainMenu->isButtonPress(3))
	{
		mCreditMenuItem->setVisible(VISIBILITY);
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
	mSettingMenuItem->setVisible(COLLAPSED);
	mCreditMenuItem->setVisible(COLLAPSED);
	mSettingSoundTurnOn = true;// demo only
}