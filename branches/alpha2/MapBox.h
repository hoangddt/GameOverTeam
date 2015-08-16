#pragma once
#include "GameObject.h"

enum MapBoxState
{
	MAP_BOX_NORMAL,
	MAP_BOX_PATH,
	MAP_BOX_FAIL,
	MAP_BOX_DANGER
};

class MapBox: public GameObject
{
private:
	Texture* mNormalTex;
	Texture* mPathTex;
	Texture* mFailTex;
	Texture* mDangerTex;
	MapBoxState mState;

	int mDangerTimeout;
	bool mIsFalling;
	bool mIsFell;
	float mStartFallingY;
	long mStartFallingTime;
	bool mFallingStarted;

	bool mIsVibrating;
	bool mIsVibrated;
	float mStartVibrateY;
	long mStartVibratingTime;
	bool mIsVibrateDown;
public:
	bool isFalling() const;
	bool isFell() const;
	bool isVibrated() const;
	void startFallingEffect();
	void setState(MapBoxState state);
	MapBoxState getState() const;
	void startVibrateEffect();
	virtual void update();
	virtual MapBox* clone() const;
	MapBox(
		Model* model, 
		Shader* shader,
		Texture* normalTex,
		Texture* pathTex,
		Texture* failTex,
		Texture* dangerTex,
		int dangerTimeout);
};
