#pragma once
#include "MapObject.h"

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

public:
	static StageInfo* getInstance();
	static void destroyInstance();

	void init(const char* PATH);
	unsigned int getCountOfLevels();
	LevelInfo getLevelInfo(int level);
};