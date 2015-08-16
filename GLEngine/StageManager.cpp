#include "StageManager.h"
#include "PlayingStageDrawer.h"
#include "SoundManager.h"
#include "GameMainMenuStage.h"
#include "GameMapStage.h"
#include "LogHeaderAndroid.h"
#include "GameLogoStage.h"
#include "Application.h"
#include "ControlButton.h"
#include "GameWinStage.h"

StageManager* StageManager::mInstance = NULL;

StageManager* StageManager::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new StageManager();
	}
	return mInstance;
}

void StageManager::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void StageManager::setStage(Stage stage)
{
	mStage = stage;
	SAFE_FREE(mStageInstace);
	SAFE_FREE(mLoadingScreen);
	mLoadingScreen = new LoadingScreen("Loading/loadingscreen.txt");
	printf("--->Load LoadingScene, prepare to choose stage\n");
	switch(stage)
	{
	case STAGE_LOGO:
		{
			GameLogoStage* logo = new GameLogoStage();
			logo->init("Logo/RM.txt", "Logo/SM.txt", NULL);
			mStageInstace = logo;
		}
		break;
	case STAGE_LOADING:
		break;
	case STAGE_MAINMENU:
		{
			printf("---->In STage_Mainmenu\n");
			GameMainMenuStage* menu = new GameMainMenuStage();
			menu->init("MainMenu/RM.txt", "MainMenu/SM.txt", NULL);
			mStageInstace = menu;
			printf("--->Out of STAGE_MAINMENU\n");
		}
		break;
	case STAGE_MAP:
		{
			GameMapStage* map = new GameMapStage();
			map->init("Map/RM.txt", "Map/SM.txt", NULL);
			mStageInstace = map;
		}
		break;
	case STAGE_EXIT:
		// This block of code below never happen
		// We has change handle exit at file GameMainMenuStage.cpp:30
		// Application::call_exit();
		// return;
	case STAGE_PLAYING1:
		{
			GamePlayingStage1* playingStage1 = new GamePlayingStage1();
			playingStage1->init("Stage1/RM.txt", 
								"Stage1/SM.txt",
								"Stage1/stage.txt", 
								SOUND_BG_STAGE1);
			mStageInstace = playingStage1;
			break;
		}
	case STAGE_PLAYING2:
		{
			GamePlayingStage2* playingStage2 = new GamePlayingStage2();
			playingStage2->init("Stage2/RM.txt", 
								"Stage2/SM.txt",
								"Stage2/stage.txt", 
								SOUND_BG_STAGE2);
			mStageInstace = playingStage2;
			break;
		}
	case STAGE_PLAYING3:
		{
			GamePlayingStage3* playingStage3 = new GamePlayingStage3();
			playingStage3->init("Stage3/RM.txt", 
								"Stage3/SM.txt",
								"Stage3/stage.txt", 
								SOUND_BG_STAGE3);
			mStageInstace = playingStage3;
			break;
		}
		break;
	case STAGE_ENDING:
		{
			GameWinStage* win = new GameWinStage();
			win->init("Win/RM.txt", 
					  "Win/SM.txt",
					  SOUND_GAMEWIN);
			mStageInstace = win;
			break;
		}
		break;
	}
	mLoading = true;
}

void StageManager::logoLogic()
{
	GameLogoStage* logo = dynamic_cast<GameLogoStage *>(mStageInstace);
	if(logo->completed())
	{
		setStage(STAGE_MAINMENU);
	}
}

void StageManager::playLogic()
{
	GamePlayingStage1* playing = dynamic_cast<GamePlayingStage1 *>(mStageInstace);
	if(playing->hasFlag(GAME_PLAYING_MOVE_END) 
		&& !playing->hasFlag(GAME_PLAYING_MOVING)
		&& !playing->hasFlag(GAME_PLAYING_POPUP))
	{
		// gameover, show popup and wait for user select
		if(playing->hasFlag(GAME_PLAYING_MOVE_FAIL))
		{
			// try again
			if(playing->hasFlag(GAME_PLAYING_TRY_AGAIN))
			{
				setStage(mStage);
			}
			// goto main menu
			else if(playing->hasFlag(GAME_PLAYING_GOTO_MAIN))
			{
				setStage(STAGE_MAINMENU);
			}
		}
		// next stage or win
		else if(playing->getLevel() >= playing->getCountOfLevels())
		{
			// next stage
			if(mStage < mMaxPlayingStage)
			{
				setStage((Stage)(mStage + 1));
			}
			// win
			else
			{
				setStage(STAGE_ENDING);
			}
		}
		// levelup
		else if(playing->hasFlag(GAME_PLAYING_MOVE_OK))
		{
			playing->nextLevel();
		}
	}
}

void StageManager::menuLogic()
{
	GameMainMenuStage* menu = dynamic_cast<GameMainMenuStage *>(mStageInstace);
	if(menu->isPlayPress())
	{
		setStage(STAGE_MAP);
	}
}

void StageManager::mapLogic()
{
	GameMapStage* map = dynamic_cast<GameMapStage *>(mStageInstace);
	if(map->isStage1Press())
	{
		setStage(STAGE_PLAYING1);
	}
	else if(map->isStage2Press())
	{
		setStage(STAGE_PLAYING2);
	}
	else if(map->isStage3Press())
	{
		setStage(STAGE_PLAYING3);
	}
}

void StageManager::render()
{
	if(mLoading)
	{
		if(mStage != STAGE_LOGO)
		{
			mLoadingScreen->render();
		}

		if(!mStageInstace->loadNext())
		{
			mLoading = false;
			mStageInstace->start();
		}
		if(mStage != STAGE_LOGO)
		{
			mLoadingScreen->setPercent(
				(float)ResourceManager::getInstance()->getCountOfLoaded() / 
				ResourceManager::getInstance()->getCountOfElements() * 
				100.0f);
			mLoadingScreen->render();
		}
	}
	else if(mStageInstace != NULL)
	{
		mStageInstace->render();
	}
}

void StageManager::update()
{
	if(mStageInstace == NULL)
	{
		return;
	}
	switch(mStage)
	{
	case STAGE_LOGO:
		logoLogic();
		break;
	case STAGE_LOADING:
		// unnecessary logic
		break;
	case STAGE_MAINMENU:
		menuLogic();
		break;
	case STAGE_MAP:
		mapLogic();
		break;
	case STAGE_EXIT:
		return;
	case STAGE_ENDING:
		// setup logic here!
		break;
	default:
		if(mStage >= STAGE_PLAYING1 && mStage <= mMaxPlayingStage)
		{
			playLogic();
		}
	}

	if(mStageInstace != NULL)
	{
		mStageInstace->update();
	}
	else
	{
		// call exit function
	}
}

void StageManager::pause()
{
	mStageInstace->pause();
}

void StageManager::destroy()
{
	SAFE_FREE(mStageInstace);
	SAFE_FREE(mLoadingScreen);
	ControlButton::destroyInstance();
}

StageManager::StageManager(void)
{
	mStageInstace = NULL;
	mMaxPlayingStage = STAGE_PLAYING3;
	mLoadingScreen = NULL;
	mLoading = false;
}

StageManager::~StageManager(void)
{
	destroy();
}
