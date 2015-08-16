#pragma once
#include "GameObject.h"

enum ArrowLocation
{
	ARROW_LEFT,
	ARROW_RIGHT,
	ARROW_FRONT
};

class ArrowObject: public GameObject
{
protected:
	float mAcceleration;
	float mVelocity0;
	//long mStartTime;
	float mStartTime;
	int mMaxDistance;
	float mLastPosition;
	ArrowLocation mLocation;

	float mRotationX;
	float mRotationY;
	float mRotationZ;
public:
	bool mIsFlying;
	bool mIsFlew;
	void setCurly(float rotationX, float rotationY, float rotationZ);
	void launch(ArrowLocation location);
	void setAcceleration(float acceleration);
	void setVelocity0(float velocity);
	virtual ArrowObject* clone() const;
	virtual void update();
	virtual void render(Camera* camera);
	ArrowObject(
		Model* model,
		Shader* shader,
		Texture* texture);
	virtual ~ArrowObject(void);
};
