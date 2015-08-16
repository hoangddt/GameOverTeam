#include "MapBox.h"
#include <math.h>
#include "../Utilities/Utilities.h"

void MapBox::setState(MapBoxState state)
{
	mState = state;
	switch(state)
	{
	case MAP_BOX_NORMAL:
		mpTextures[0] = mNormalTex;
		break;
	case MAP_BOX_PATH:
		mpTextures[0] = mPathTex;
		break;
	case MAP_BOX_FAIL:
		mpTextures[0] = mFailTex;
		break;
	case MAP_BOX_DANGER:
		mpTextures[0] = mDangerTex;
		break;
	}
}

MapBoxState MapBox::getState() const
{
	return mState;
}

void MapBox::startVibrateEffect()
{
	static float min = mPostion.y - 0.08f;

	//clock_t now = clock();
	//now *= 1000.0f / CLOCKS_PER_SEC;
	// init vibrate effect
	if(!mIsVibrating)
	{
		//mStartVibratingTime = now;
		mStartVibratingTime = 0.0f;
		mIsVibrating = true;
		mIsVibrated = false;
		mIsVibrateDown = true;
		mStartVibrateY = mPostion.y;
	}
	// vibrating...
	else
	{
		//float time = (now - mStartVibratingTime) / 1000.0f;
		mStartVibratingTime += 0.04f;
		float dental = 1.0f * mStartVibratingTime;
		if(mIsVibrateDown)
		{
			if(mStartVibrateY - dental > min)
			{
				mPostion.y = mStartVibrateY - dental;
			}
			else
			{
				mIsVibrateDown = false;
			}
		}
		else
		{
			if(mPostion.y + dental <= mStartVibrateY)
			{
				mPostion.y = mStartVibrateY + dental;
			}
			else
			{
				mPostion.y = mStartVibrateY;
				mIsVibrating = false;
				mIsVibrated = true;
			}
		}
	}
}

void MapBox::startFallingEffect()
{
	mFallingStarted = true;
}

void MapBox::update()
{
	GameObject::update();
	if(mState == MAP_BOX_DANGER && mIsVisible && mFallingStarted)
	{
		//clock_t now = clock();
		//now *= 1000.0f / CLOCKS_PER_SEC;
		
		// init falling effect
		if(!mIsFalling){
			// the first time
			if(mStartFallingTime == 0.0f)
			{
				mIsVibrating = false;
				mIsFell = false;
				//mStartFallingTime = now;
				mStartFallingTime = 0.001f;
				mStartFallingY = mPostion.y;
			}
			// wait until timeout
			else if(mStartFallingTime > mDangerTimeout)
			{
				mIsFalling = true;
				mStartFallingTime = 0.0f;
			}
			else
			{
				mStartFallingTime += 10.0f;
			}
		}
		// start falling effect
		else
		{
			// falling...
			if(mPostion.y > -2)
			{
				//float time = (now - mStartFallingTime) / 1000.0f;
				mStartFallingTime += 0.02f;
				mPostion.y = mStartFallingY + -0.5f * MATH_GRAVITY * mStartFallingTime * mStartFallingTime;
			}
			// falled
			else
			{
				mIsFalling = false;
				mIsFell = true;
				mIsVisible = false;

				mIsVibrated = true;
			}
		}
	}
	if(mIsVibrating)
	{
		startVibrateEffect();
	}
}

MapBox* MapBox::clone() const
{
	MapBox* newBox = new MapBox(
		mpModel,
		mpShader,
		mNormalTex,
		mPathTex,
		mFailTex,
		mDangerTex,
		mDangerTimeout);
	newBox->mState = mState;

	return newBox;
}

bool MapBox::isFalling() const
{
	return mIsFalling;
}

bool MapBox::isFell() const
{
	return mIsFell;
}

bool MapBox::isVibrated() const
{
	return mIsVibrated;
}

MapBox::MapBox(
			   Model* model, 
			   Shader* shader,
			   Texture* normalTex,
			   Texture* pathTex,
			   Texture* failTex,
			   Texture* dangerTex,
			   int dangerTimeout)
			   :GameObject()
{
	mIsFalling = false;
	mIsFell = false;
	mFallingStarted = false;
	mStartFallingTime = 0;

	mIsVibrating = false;
	mIsVibrated = false;

	mpModel = model;
	mpShader = shader;
	mCountOfTextures = 1;
	mpTextures = new Texture*[mCountOfTextures];
	mpTextures[0] = normalTex;
	mCountOfCubetexs = 0;
	mpCubetexs = NULL;

	mNormalTex = normalTex;
	mPathTex = pathTex;
	mFailTex = failTex;
	mDangerTex = dangerTex;

	mDangerTimeout = dangerTimeout;
}