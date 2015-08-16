#pragma once
#include "MonsterEnemy.h"
#include "MapObject.h"
#include "StageInfo.h"

#define ENEMY_LEFT	-4.0f
#define ENEMY_RIGHT	4.0f

class EnemyManager
{
private:
	Model* mModel;
	Shader* mShader;
	Texture* mTexture;
	MonsterEnemy** mEnemies;
	int mCountOfEnemies;
	bool mReady;
	bool mStarted;
	bool mIsLastDemonLeft;
	long mTimeout;
	long mStartTime;
	
	LevelInfo* mLevelInfo;
public:
	const GameObject* mCharacter;
	bool mIsVisible;
	void render();
	void update();
	void startEnemy(bool start, LevelInfo* levelInfo);
	bool impacted() const;
	EnemyManager(Model* model, 
				 Shader* shader, 
				 Texture* texture,
				 const GameObject* character);
	~EnemyManager(void);
};
