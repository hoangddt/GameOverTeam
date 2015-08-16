#pragma once

#include "Map.h"

class GameData {
private:
	static GameData* mInstance;
	char* mFileName;

	// cac giu lieu can luu vao file
	int mCurrStage; // stage dang choi hien tai
	int mSoundSetting; // 0 -> sound off, 1 -> sound on
	int mGameMode; // 1 -> easy, 2 -> medium, 3 -> hard

	GameData();
	GameData(int stage, int soundSetting, int mode);
	~GameData();

public:
	static GameData* getInstance();
	static void destroyInstance();

	void init(const char* fileName); // khoi tao GameData tu file
	void saveData(); // luu tat ca du lieu hien tai vao file
	int getCurrStage();
	void setCurrStage(int stage);
	int getSoundSetting();
	void setSoundSetting(int setting);
	GameMode getGameMode();
	void setGameMode(GameMode gameMode);
};