#include "GamePlayingStage3.h"
#include "ControlButton.h"
#include <math.h>

void GamePlayingStage3::jump(bool init)
{
	static float v0 = 50.5f;
	static float startTime;
	static float startY;
	static float sinAlpha;
	static float hmax = 2.0f;
	static float acceleration = MATH_GRAVITY;
	long now = clock() * 1000.0f / CLOCKS_PER_SEC;

	if(init)
	{
		mIsJumping = true;
		mIsJumpEnd = false;
		startTime = now;
		startY = mCharacter->mPostion.y;
		sinAlpha = sqrt(2.0f * acceleration * hmax) / v0;

		SoundManager::getInstance()->play(SOUND_JUMPSTART);

		addFlag(GAME_PLAYING_MOVING);
		addFlag(GAME_PLAYING_JUMP_UP);
	}

	float time = (now - startTime) / 500.0f;

	float newY = startY + v0 * time * sinAlpha - 0.5 * acceleration * time * time;

	// jump y
	if(newY < startY)
	{
		mCharacter->mPostion.y = startY;
		
		//removeFlag(GAME_PLAYING_MOVING);
		addFlag(GAME_PLAYING_JUMP_END);

		MapObject* mapObj = mLevelInfo.mMapObject;
		Vector2 curPos = mapObj->getCurrentPosition();

		// whenmove right, add move_ok and remove move_fail,
		// then check for finish current level yet
		if(mapObj->isValidCurPosition())
		{
			addFlag(GAME_PLAYING_MOVE_OK);
			removeFlag(GAME_PLAYING_MOVE_FAIL);
			if(curPos.x == 0)
			{
				addFlag(GAME_PLAYING_MOVE_END);
			}
		}
		// when move wrong, add move_fail, remove move_ok
		// and finish current level by add move_end
		else
		{
			addFlag(GAME_PLAYING_MOVE_FAIL);
			addFlag(GAME_PLAYING_MOVE_END);
			removeFlag(GAME_PLAYING_MOVE_OK);
		}
 
		SoundManager::getInstance()->play(SOUND_JUMPEND);

		//checkNewState();

		mapObj->getCurBox()->startVibrateEffect();

		removeFlag(GAME_PLAYING_JUMP_UP);

		mIsJumping = false;
	}
	else
	{
		mCharacter->mPostion.y = newY;
	}
}

void GamePlayingStage3::move(Direction direction)
{
	GamePlayingStage2::move(direction);
}

void GamePlayingStage3::checkNewState()
{
	if(hasFlag(GAME_PLAYING_MOVE_END))
	{
		mEnemyManager->mIsVisible = false;
	}
	GamePlayingStage2::checkNewState();
}

void GamePlayingStage3::setLevel(int level)
{
	GamePlayingStage2::setLevel(level);
	mLevelInfo.mMapObject->setSupportFalling(false);
	mEnemyManager->mCharacter = mCharacter;
	mEnemyManager->startEnemy(true, &mLevelInfo);
	mIsJumpEnd = true;
	mIsJumping = false;
}

void GamePlayingStage3::ready()
{
	GamePlayingStage2::ready();
	mEnemyManager->mIsVisible = true;
}

void GamePlayingStage3::start()
{
	mEnemyManager = new EnemyManager(
		mResourceManager->getModelById(RES_BALL_MODEL_ID),
		mResourceManager->getShaderById(RES_BALL_SHADER_ID),
		mResourceManager->getTextureById(RES_BALL_TEXTURE_ID),
		mCharacter);
	GamePlayingStage2::start();
}

void GamePlayingStage3::update()
{
	if(!mLoaded)
	{
		return;
	}
	GamePlayingStage2::update();
	if(mPopupMenu->opened())
	{
		return;// only popup has been update, other will be disabled
	}

	if(mEnemyManager->mIsVisible)
	{
		mEnemyManager->update();
	}

	if(mIsReady && !hasFlag(GAME_PLAYING_MOVE_FAIL))
	{
		if(mIsJumpEnd)
		{
			if(ControlButton::getInstance()->checkClick() == BUTTON_UP)
			{
				ControlButton::getInstance()->clear();
				jump(true);
			}
		}
		else
		{
			if(mIsJumping)
			{
				jump(false);
			}
			else
			{
				MapBox* box = mLevelInfo.mMapObject->getCurBox();
				if(box->isVibrated())
				{
					mIsJumpEnd = true;
					mIsJumping = false;
				}
			}
		}

		if(mEnemyManager->impacted())
		{
			/*mIsReady = false;
			SoundManager::getInstance()->play(SOUND_IMPACT);
			addFlag(GAME_PLAYING_MOVE_FAIL);
			addFlag(GAME_PLAYING_MOVE_END);
			removeFlag(GAME_PLAYING_MOVE_OK);
			checkNewState();*/
		}
	}
}

void GamePlayingStage3::render()
{
	if(mLoaded)
	{
		mEnemyManager->render();
	}
	GamePlayingStage2::render();
}

void GamePlayingStage3::destroy()
{
	SAFE_FREE(mEnemyManager);
	GamePlayingStage2::destroy();
}

GamePlayingStage3::GamePlayingStage3(void)
{
	SoundManager::getInstance()->stop(SOUND_BG_STAGE2);
	mIsJumpEnd = true;
	mIsJumping = false;
	mStageId = 3;
	//mShowPathTimeout = 3500;
}

GamePlayingStage3::~GamePlayingStage3(void)
{
	destroy();
}
