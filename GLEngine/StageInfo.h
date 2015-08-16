#pragma once
#include "MapObject.h"
#include "../Utilities/Vector3.h"
#include "Map.h"

struct LevelInfo
{
	LevelInfo()
	{
		mMapObject = NULL;
	}
	
	int mLevel;
	int mMap[2];
	int mModelId;
	int mTexturesId[4];
	int mDangerTimeout;
	int mTimeShoot;
	Vector3 mRotation;
	int mSpeedOfEnemy;
	MapObject *mMapObject;
};

class StageInfo
{
private:
	StageInfo();
	~StageInfo();
	StageInfo(const StageInfo&) {}
	StageInfo& operator=(const StageInfo&) {}

	static StageInfo* mInstance;

	unsigned int mNumberOfLevel;
	LevelInfo* mLevelInfos;
	void genMapObject(LevelInfo *level);
	MapObject *mLastMapObject;
	GameMode mGameMode;

public:
	static StageInfo* getInstance();
	static void destroyInstance();

	void init(const char* PATH);
	unsigned int getCountOfLevels();
	LevelInfo getLevelInfo(int level);
	GameMode getGameMode();
	void setGameMode(GameMode mode);
};