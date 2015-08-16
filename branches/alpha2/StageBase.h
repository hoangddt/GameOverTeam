#pragma once
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"

class StageBase
{
protected:
	SceneManager* mSceneManager;
	ResourceManager* mResourceManager;
	const char* mBackgroundSoundId;
	bool mLoaded;
public:
	virtual void init(const char* rmFile, 
					  const char* smFile, 
					  const char* bgSoundId);
	virtual void start();
	virtual bool loadNext();
	virtual void render();
	virtual void update();
	virtual void destroy();
	StageBase(void);
	virtual ~StageBase(void);
};
