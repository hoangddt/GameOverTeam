#include "StageInfo.h"
#include "FileManager.h"
#include "Map.h"
#include "ResourceManager.h"
#include <cstdlib>


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
	printf("#LEVELs %d", mNumberOfLevel);
	mLevelInfos = new LevelInfo[mNumberOfLevel];
	
	for (int i = 0; i < mNumberOfLevel; i++)
	{
		fscanf(fp, "%d. %d %d, ", &mLevelInfos[i].mLevel, &mLevelInfos[i].mMap[0], &mLevelInfos[i].mMap[1]);
		fscanf(fp, "%d, ", &mLevelInfos[i].mModelId);
		fscanf(fp, "%d %d %d", &mLevelInfos[i].mTexturesId[0], &mLevelInfos[i].mTexturesId[1], &mLevelInfos[i].mTexturesId[2]);
		fscanf(fp, "\n");
		printf("%d. %d %d, %d, %d %d %d\n",mLevelInfos[i].mLevel, mLevelInfos[i].mMap[0], mLevelInfos[i].mMap[1],
											mLevelInfos[i].mModelId,
											mLevelInfos[i].mTexturesId[0], mLevelInfos[i].mTexturesId[1], mLevelInfos[i].mTexturesId[2]
											);
		printf("\n");
	}
	FileManager::getInstance()->closeFile(fp);
}

void StageInfo::genMapObject(LevelInfo *level)
{
	int row = level->mMap[0],
		col = level->mMap[1];
	int** map = Map::getInstance()->genMap(row, col);

	Model *model = ResourceManager::getInstance()->getModelById(level->mModelId);
	Shader *shader = ResourceManager::getInstance()->getShaderById(0);
	Texture *normalTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[0]);
	Texture *pathTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[1]);
	Texture *failTex = ResourceManager::getInstance()->getTextureById(level->mTexturesId[2]);
	
	level->mMapObject = new MapObject((const int**)map, row, col, model, shader, normalTex, pathTex, failTex, NULL, 0);

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