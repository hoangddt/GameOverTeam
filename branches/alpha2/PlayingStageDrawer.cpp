#include "PlayingStageDrawer.h"
#include "Lib.h"
#include "LogHeaderAndroid.h"

PlayingStageDrawer* PlayingStageDrawer::mInstance = NULL;

PlayingStageDrawer* PlayingStageDrawer::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new PlayingStageDrawer();
	}
	return mInstance;
}

void PlayingStageDrawer::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void PlayingStageDrawer::setLevel(int level)
{

}

void PlayingStageDrawer::setPlayState(GameState state)
{
#if WIN32
	return;
	switch(state)
	{
	case GAME_OVER:
		MessageBoxA(NULL, "R.I.P", "GAMEOVER TEAM", 0);
		break;
	case GAME_WIN_STAGE:
		MessageBoxA(
			NULL, 
			"Win stage 1! next your challenge is stage 2.\nIt will harder and very crazy!", 
			"GAMEOVER TEAM", 
			0);
		break;
	case GAME_LEVEL_UP:
		MessageBoxA(NULL, "Bummmm bummm bummm...Level up!!!!!!!", "GAMEOVER TEAM", 0);
		break;
	case GAME_WIN:
		MessageBoxA(NULL, "Winnnnnnn!!!!!!!\nYou are crazy...", "GAMEOVER TEAM", 0);
		break;
	}
#endif
}

void PlayingStageDrawer::render()
{
}

void PlayingStageDrawer::update()
{
}

void PlayingStageDrawer::destroy()
{
}

PlayingStageDrawer::~PlayingStageDrawer()
{
}