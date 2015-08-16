#include "ArrowObject.h"
#include "LogHeaderAndroid.h"

void ArrowObject::launch(ArrowLocation location)
{
	mIsFlew = false;
	mIsFlying = true;
	mIsVisible = true;
	mLocation = location;
	//mStartTime = clock() * 1000.0f / CLOCKS_PER_SEC;
	mStartTime = 0.0f;
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

void ArrowObject::setCurly(float rotationX, float rotationY, float rotationZ)
{
	mRotationX = rotationX;
	mRotationY = rotationY;
	mRotationZ = rotationZ;
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

	//clock_t now = clock();
	//float time = (now * 1000.0f / CLOCKS_PER_SEC - mStartTime) / 1000.0f;
	mStartTime += 0.01f;
	float dental = mVelocity0 * mStartTime + 0.5f * mAcceleration * mStartTime * mStartTime;
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

	mRotation.z += mRotationZ;
	mRotation.x += mRotationX;
	mRotation.y += mRotationY;

	if(completed)
	{
		mIsFlew = true;
		mIsFlying = false;
		mIsVisible = false;
		mRotation.z = 0.0f;
		mRotation.x = 0.0f;
		mRotation.y = 0.0f;
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

	mRotationX = 0.0f;
	mRotationY = 0.0f;
	mRotationZ = 0.0f;

	mIsFlew = false;
	mIsFlying = false;
	mAcceleration = 1.0f;
	mVelocity0 = 1.0f;
	mLocation = ARROW_LEFT;
	mStartTime = 0;
	mMaxDistance = 10;
	mLastPosition = 0.0f;
}

ArrowObject::~ArrowObject(void)
{
}