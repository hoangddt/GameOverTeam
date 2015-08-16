#include "GameData.h"
#include <string.h>
#include "FileManager.h"
#include "Lib.h"
#include "SoundManager.h"
#include "LogHeaderAndroid.h"
#include "StageInfo.h"

GameData* GameData::mInstance = NULL;

GameData* GameData::getInstance() {
	if (mInstance == NULL) {
		mInstance = new GameData;
	}
	return mInstance;
}

void GameData::destroyInstance() {
	SAFE_FREE(mInstance);
}

GameData::GameData() {
}

GameData::GameData(int stage, int soundSetting, int mode) {
	this->mCurrStage = stage;
	this->mSoundSetting = soundSetting;
	this->mGameMode = mode;
}

GameData::~GameData() {
	if (this->mFileName != NULL) {
		delete this->mFileName;
		mFileName = NULL;
		this->mCurrStage = 0;
		this->mSoundSetting = 1;
		this->mGameMode = 2;
	}
}

// khoi tao GameData tu file
void GameData::init(const char* fileName) {
	//printf("Gamedata init() : \n");
	if (fileName == NULL) {
		printf("GameData class. FileName is NULL");
	} else {
		this->mFileName = new char[ strlen(fileName) + 1 ];
		strcpy(this->mFileName, fileName);

		int stage		 = 1;
		int soundSetting = 1;
		int gameMode	 = 1;

		FILE *pf = FileManager::getInstance()->openFile(this->mFileName, "r");

		printf("GameData::init() 56 filename: %s\n", this->mFileName);

		if(pf == NULL)
		{
			printf("GameData::init(): Game data not found! creating it!\n");
		}
		else
		{
			// doc du lieu tu file GameData
			// format : stage soundsetting gamemode
			fscanf(pf, "%d %d %d", &stage, &soundSetting, &gameMode);

			// gan du lieu
			this->mCurrStage	= stage;
			this->mGameMode		= gameMode;
			this->mSoundSetting = soundSetting;

			printf("GameData: init(): 73: currStage:%d  soundsetting:%d  gamemode:%d", this->mCurrStage, this->mSoundSetting, this->mGameMode);

			FileManager::getInstance()->closeFile(pf);
		}
	}
}

// luu tat ca du lieu hien tai vao file
void GameData::saveData() {

	// cap nhat du lieu 
	// updateData();

	printf("GameData: saveData(): 93: currStage:%d  soundsetting:%d  gamemode:%d", this->mCurrStage, this->mSoundSetting, this->mGameMode);

	FILE *pf = FileManager::getInstance()->openFile(this->mFileName, "w");

	// ghi du lieu vao file GameData
	// format : stage soundsetting gamemode
	fprintf(pf, "%d %d %d", this->mCurrStage, this->mSoundSetting, this->mGameMode);

	FileManager::getInstance()->closeFile(pf);
}

int GameData::getCurrStage() {
	return this->mCurrStage;
}

void GameData::setCurrStage(int stage) {
	this->mCurrStage = stage;
	this->saveData();
}

int GameData::getSoundSetting() {
	return this->mSoundSetting;
}

void GameData::setSoundSetting(int settingMode) {
	this->mSoundSetting = settingMode;
	this->saveData();
}

GameMode GameData::getGameMode() {
	switch(this->mGameMode) {
		case 1:
			return EASY;
			break;
		case 2:
			return MEDIUM;
			break;
		case 3:
			return HARD;
			break;
		default:
			return HARD;
			break;
	}
}

void GameData::setGameMode(GameMode gameMode) {
	switch(gameMode) {
		case EASY:
			this->mGameMode = 1;
			break;
		case MEDIUM:
			this->mGameMode = 2;
			break;
		case HARD:
			this->mGameMode = 3;
			break;
		default:
			this->mGameMode = 3;
			break;
	}
	this->saveData();
}