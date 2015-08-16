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
public:
	void setState(MapBoxState state);
	virtual void update();
	virtual MapBox* clone() const;
	MapBox(
		Model* model, 
		Shader* shader,
		Texture* normalTex,
		Texture* pathTex,
		Texture* failTex,
		Texture* dangerTex);
	virtual ~MapBox(void);
};
