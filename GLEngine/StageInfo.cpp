#include "StageInfo.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include <cstdlib>
#include "LogHeaderAndroid.h"
#include "GameData.h"

#define MAP_SHADER_ID 3

StageInfo* StageInfo::mInstance = NULL;

StageInfo::StageInfo()
{
	mLevelInfos = NULL;
	mLastMapObject = NULL;
}

StageInfo::~StageInfo()
{
	FREE_1D_ARRAY(mLevelInfos);
	SAFE_FREE(mLastMapObject);
}

StageInfo* StageInfo::getInstance()
{
	if (mInstance == NULL)
		mInstance = new StageInfo();
	return mInstance;
}

void StageInfo::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void StageInfo::init(const char* PATH)
{
	FILE *fp = FileManager::getInstance()->openFile(PATH, "r");
	fscanf(fp,"#LEVELS %d\n", &mNumberOfLevel);
	// printf("#LEVELs %d\n", mNumberOfLevel);
	mLevelInfos = new LevelInfo[mNumberOfLevel];
	
	for (int i = 0; i < mNumberOfLevel; i++)
	{
		// printf("Loops: %d\n", i);
		// map matrix
		fscanf(fp, "%d. %d %d, ", 
			&mLevelInfos[i].mLevel,  /* level id */
			&mLevelInfos[i].mMap[0], /* matrix_rows */
			&mLevelInfos[i].mMap[1]);/* matrix_cols */
		// mapbox model
		fscanf(fp, "%d, ", &mLevelInfos[i].mModelId);
		// mapbox textures for each state
		fscanf(fp, "%d %d %d %d, ", 
			&mLevelInfos[i].mTexturesId[0], /* normal texture */
			&mLevelInfos[i].mTexturesId[1], /* path texture */
			&mLevelInfos[i].mTexturesId[2], /* fail texture */
			&mLevelInfos[i].mTexturesId[3]);/* danger texture */
		// mapbox danger timeout
		fscanf(fp, "%d, ", &mLevelInfos[i].mDangerTimeout);

		fscanf(fp,"%d, ", &mLevelInfos[i].mTimeShoot);
		fscanf(fp, "%f %f %f, ",
			&mLevelInfos[i].mRotation.x,
			&mLevelInfos[i].mRotation.y,
			&mLevelInfos[i].mRotation.z);
		fscanf(fp, "%d", &mLevelInfos[i].mSpeedOfEnemy);
		// next line
		fscanf(fp, "\n");
		/*
		printf("%d. %d %d, %d, %d %d %d\n",mLevelInfos[i].mLevel, mLevelInfos[i].mMap[0], mLevelInfos[i].mMap[1],
											mLevelInfos[i].mModelId,
											mLevelInfos[i].mTexturesId[0], mLevelInfos[i].mTexturesId[1], mLevelInfos[i].mTexturesId[2]
											);
		
		printf("\n");
		*/
		
	}
	FileManager::getInstance()->closeFile(fp);

	// get gamemode from GameData
	this->setGameMode( GameData::getInstance()->getGameMode() );

	printf("End of StageInfo init() \n");
}

void StageInfo::genMapObject(LevelInfo *level)
{
	int row = level->mMap[0],
		col = level->mMap[1];
	GameMode mode = this->getGameMode();
	int** map = Map::getInstance()->genMap(row, col, mode);

	Model *model = ResourceManager::getInstance()->getModelById(level->mModelId);
	Shader *shader = ResourceManager::getInstance()->getShaderById(MAP_SHADER_ID);
	Texture *normalTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[0]);
	Texture *pathTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[1]);
	Texture *failTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[2]);
	Texture *dangerTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[3]);
	int dangerTimeout = level->mDangerTimeout;
	
	level->mMapObject = new MapObject(
		(const int**)map, 
		row, 
		col, 
		model, 
		shader, 
		normalTex, 
		pathTex, 
		failTex, 
		dangerTex, 
		dangerTimeout);

	Map::getInstance()->freeMap(map, row, col);
	Map::destroyInstance();
}

unsigned int StageInfo::getCountOfLevels()
{
	return mNumberOfLevel;
}

LevelInfo StageInfo::getLevelInfo(int level)
{
	if (level > 0 && level <= mNumberOfLevel)
	{
		SAFE_FREE(mLastMapObject);
		genMapObject(mLevelInfos + (level - 1));
		mLastMapObject = mLevelInfos[level-1].mMapObject;
		return mLevelInfos[level-1];
	}
	else
	{
		printf("In StageInfo class. You just passed invalid level\n");
	}
}

GameMode StageInfo::getGameMode()
{
	return this->mGameMode;
}

void StageInfo::setGameMode(GameMode mode) 
{
	this->mGameMode = mode;
}

/* How to use 
int main()
{

	FileManager::getInstance()->init("../Resources");
	ResourceManager::getInstance()->init("RM.txt");
	StageInfo::getInstance()->init("Stage1/stage.txt");
	
	LevelInfo levelTemp = StageInfo::getInstance()->getLevelInfo(1);
	levelTemp;

	LevelInfo levelTemp2 = StageInfo::getInstance()->getLevelInfo(2);
	levelTemp2;

	FileManager::destroyInstance();
	StageInfo::destroyInstance();
	ResourceManager::destroyInstance();
	return 0;
}
*/