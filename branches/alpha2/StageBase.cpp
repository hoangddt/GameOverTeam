#include "StageBase.h"
#include "LogHeaderAndroid.h"

bool StageBase::loadNext()
{
	bool ret = mResourceManager->loadNext();
	mLoaded = !ret;
	return ret;
}

void StageBase::init(const char* rmFile, 
					 const char* smFile, 
					 const char* bgSoundId)
{
	printf("In StageBase::init class:\n");
	ResourceManager::destroyInstance();
	mResourceManager = ResourceManager::getInstance();
	mResourceManager->init(rmFile);
	printf("End init rmFile: %s\n", rmFile);
	SceneManager::destroyInstance();
	mSceneManager = SceneManager::getInstance();
	mSceneManager->init(smFile);
	printf("End init smFile: %s\n", smFile);
	mBackgroundSoundId = bgSoundId;
	printf("In StageBase::init  end class:\n");
}

void StageBase::start()
{
	if(mBackgroundSoundId != NULL)
	{
		SoundManager::getInstance()->play(mBackgroundSoundId, true);
	}
}

void StageBase::render()
{
	mSceneManager->render();
}

void StageBase::update()
{
	mSceneManager->update();
}

void StageBase::destroy()
{
	SceneManager::destroyInstance();
	ResourceManager::destroyInstance();
}

StageBase::StageBase(void)
{
	mLoaded = false;
}

StageBase::~StageBase(void)
{
	destroy();
}
