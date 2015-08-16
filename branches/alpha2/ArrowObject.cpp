#include "ArrowObject.h"
#include "LogHeaderAndroid.h"

void ArrowObject::launch(ArrowLocation location)
{
	mIsFlew = false;
	mIsFlying = true;
	mLocation = location;
	mStartTime = clock();
	switch(location)
	{
	case ARROW_FRONT:
		mLastPosition = mPostion.z;
		break;
	case ARROW_LEFT:
		mLastPosition = mPostion.x;
		break;
	case ARROW_RIGHT:
		mLastPosition = mPostion.x;
		break;
	}
}

void ArrowObject::setAcceleration(float acceleration)
{
	mAcceleration = acceleration;
}

void ArrowObject::setVelocity0(float velocity)
{
	mVelocity0 = velocity;
}

ArrowObject* ArrowObject::clone() const
{
	ArrowObject* obj = new ArrowObject(mpModel, mpShader, mpTextures[0]);
	obj->mAcceleration = mAcceleration;
	obj->mVelocity0 = mVelocity0;
	obj->mIsFlying = mIsFlying;
	obj->mIsFlew = mIsFlew;
	obj->mLocation = mLocation;
	return obj;
}

void ArrowObject::render(Camera* camera)
{
	GameObject::render(camera);
}

void ArrowObject::update()
{
	GameObject::update();

	if(!mIsFlying)
	{
		return;
	}

	clock_t now = clock();
	float time = (now - mStartTime) / 1000.0f;
	float dental = mVelocity0 * time + 0.5f * mAcceleration * time * time;
	bool completed = true;

	switch(mLocation)
	{
	case ARROW_FRONT:
		if(mPostion.z - mLastPosition < mMaxDistance)
		{
			mPostion.z = mLastPosition + dental;
			completed = false;
		}
		break;
	case ARROW_LEFT:
		if(mPostion.x - mLastPosition < mMaxDistance)
		{
			mPostion.x = mLastPosition + dental;
			completed = false;
		}
		break;
	case ARROW_RIGHT:
		if(mLastPosition - mPostion.x < mMaxDistance)
		{
			mPostion.x = mLastPosition - dental;
			completed = false;
		}
		break;
	}

	if(completed)
	{
		mIsFlew = true;
		mIsFlying = false;
		mIsVisible = false;
	}
}

ArrowObject::ArrowObject(Model* model,
						 Shader* shader,
						 Texture* texture)
						 :GameObject()
{
	mpModel = model;
	mpShader = shader;
	mCountOfTextures = 1;
	mpTextures = new Texture* [mCountOfTextures];
	mpTextures[0] = texture;
	mCountOfCubetexs = 0;
	mpCubetexs = NULL;

	mIsFlew = false;
	mIsFlying = false;
	mAcceleration = 9.0f;
	mVelocity0 = 1.0f;
	mLocation = ARROW_LEFT;
	mStartTime = 0;
	mMaxDistance = 10;
	mLastPosition = 0.0f;
}

ArrowObject::~ArrowObject(void)
{
}