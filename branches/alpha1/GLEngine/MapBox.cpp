#include "MapBox.h"

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

void MapBox::update()
{
	GameObject::update();
	if(mState == MAP_BOX_DANGER && mIsVisible)
	{
		static int timeout = 0;
		static float time = 0.0f;
		static float gravity = -9.8f;
		static float startY = 0.0f;
		
		if(timeout < mDangerTimeout)
		{
			startY = mPostion.y;
			timeout += 10;
		}
		else if(mPostion.y > -1)
		{
			mPostion.y = startY + 0.5f * gravity * time * time;
		}
		else
		{
			timeout = 0;
			mIsVisible = false;
		}
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
		mDangerTex);
	newBox->mDangerTimeout = mDangerTimeout;
	newBox->mState = mState;

	return newBox;
}

MapBox::MapBox(
			   Model* model, 
			   Shader* shader,
			   Texture* normalTex,
			   Texture* pathTex,
			   Texture* failTex,
			   Texture* dangerTex)
			   :GameObject()
{
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

	mDangerTimeout = 5000;
}

MapBox::~MapBox(void)
{
}
