#include "EnemyManager.h"
#include "GamePlayingStage1.h"

void EnemyManager::render()
{
	if(mIsVisible)
	{
		for(int i = 0; i < mCountOfEnemies; i++)
		{
			mEnemies[i]->render();
		}
	}
}

void EnemyManager::update()
{
	if(!mStarted)
	{
		return;
	}
	
	if(!mReady)
	{
		long now = clock() * 1000.0f / CLOCKS_PER_SEC;
		if(now - mStartTime > mTimeout)
		{
			mReady = true;
		}
	}
	else
	{
		for(int i = 0; i < mCountOfEnemies; i++)
		{
			mEnemies[i]->update();
		}
	}
}

bool EnemyManager::impacted() const
{
	if(mStarted)
	{
		Vector2 cur = mLevelInfo->mMapObject->getCurrentPosition();
		if(cur.x >= mCountOfEnemies)
		{
			return false;
		}
		return mEnemies[(int)cur.x]->impacted(mCharacter);
	}
	return false;
}

void EnemyManager::startEnemy(bool start, LevelInfo* levelInfo)
{
	mLevelInfo = levelInfo;
	mStarted = start;
	float startY = mCharacter->mPostion.y - CHARACTER_DELTA_Y + 0.32f;

	FREE_2D_ARRAY(mEnemies, mCountOfEnemies);

	mCountOfEnemies = levelInfo->mMapObject->getRows() - 1;
	mEnemies = new MonsterEnemy* [mCountOfEnemies];
	MapBox* box = levelInfo->mMapObject->getBox(0, 0);

	float height = box->getActualHeight();
	Vector3 pos(ENEMY_LEFT, startY, levelInfo->mMapObject->mPosition.z + mCountOfEnemies * height / 2.0f);

	srand(time(NULL));	
	for(int i = mCountOfEnemies - 1; i >= 0; i--)
	{
		mEnemies[i] = new MonsterEnemy(mModel, mShader, mTexture);
		mEnemies[i]->setStartAfter(rand() % 2500);
		mEnemies[i]->setConfig(levelInfo->mSpeedOfEnemy, levelInfo->mRotation);
		pos.z -= height;

		if(i % 2)
		{
			pos.x = ENEMY_LEFT;
			mEnemies[i]->startAt(pos, ARROW_LEFT);
		}
		else
		{
			pos.x = ENEMY_RIGHT;
			mEnemies[i]->startAt(pos, ARROW_RIGHT);
		}
	}

	mTimeout = levelInfo->mTimeShoot;
	mStartTime = clock() * 1000.0f / CLOCKS_PER_SEC;
}

EnemyManager::EnemyManager(Model* model, 
						   Shader* shader, 
						   Texture* texture,
						   const GameObject* character)
{
	mCharacter = character;
	mModel = model;
	mShader = shader;
	mTexture = texture;

	mReady = false;
	mStarted = false;
	mIsVisible = true;
	mTimeout = 0;// 1s

	mEnemies = NULL;
	mCountOfEnemies = 0;
}

EnemyManager::~EnemyManager(void)
{
	FREE_2D_ARRAY(mEnemies, mCountOfEnemies);
}
