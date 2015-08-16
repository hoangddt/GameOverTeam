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
	bool mIsFlying;
	bool mIsFlew;
	float mAcceleration;
	float mVelocity0;
	long mStartTime;
	int mMaxDistance;
	float mLastPosition;
	ArrowLocation mLocation;
public:
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
