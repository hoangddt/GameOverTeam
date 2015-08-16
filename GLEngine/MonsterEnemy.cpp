#include "MonsterEnemy.h"

void MonsterEnemy::render()
{
	if(mArrow->isVisible())
	{
		mArrow->render(mCamera);
	}
}

void MonsterEnemy::update()
{
	if(mArrow->isVisible())
	{
		mArrow->update();
	}
	else if(!mArrow->mIsFlying)
	{
		mArrow->mPostion = mStartLocation;
		mArrow->launch(mLocation);
	}

	if(mStartAfter < 0)
	{
		return;		
	}

	long now = clock() * 1000.0f / CLOCKS_PER_SEC;
	if(now - mStartTime > mStartAfter)
	{
		mArrow->launch(mLocation);
		mStartAfter = -1;
	}
}

void MonsterEnemy::setStartAfter(int miliseconds)
{
	mStartAfter = miliseconds;
}

void MonsterEnemy::startAt(const Vector3& pos, ArrowLocation location)
{
	mStartLocation = pos;
	mArrow->mPostion = pos;
	mArrow->setVisible(true);
	mLocation = location;
	mStartTime = clock() * 1000.0f / CLOCKS_PER_SEC;
}

void MonsterEnemy::setLevel(MonsterLevel level)
{
	mLevel = level;
	switch(level)
	{
	case MONSTER_LEVEL_0:
		mArrow->setVelocity0(7.2f);
		mArrow->setAcceleration(0.0f);
		break;
	case MONSTER_LEVEL_1:
		mArrow->setVelocity0(9.5f);
		mArrow->setAcceleration(0.0f);
		break;
	case MONSTER_LEVEL_2:
		mArrow->setVelocity0(12.0f);
		mArrow->setAcceleration(0.0f);
		break;
	}
}

void MonsterEnemy::setConfig(int level, const Vector3& rotation)
{
	setLevel((MonsterLevel)level);
	mArrow->mRotation = rotation;
}

bool MonsterEnemy::isVisible() const
{
	return mArrow->isVisible();
}

bool MonsterEnemy::impacted(const GameObject* character) const
{
	if(mArrow->isVisible())
	{
		return character->impactWith(mArrow, 0.8f);
	}
	return false;
}

MonsterEnemy::MonsterEnemy(Model* model, 
						   Shader* shader, 
						   Texture* texture)
{
	mArrow = new ArrowObject(model, shader, texture);
	mArrow->mScaling = Vector3(0.5f, 0.5f, 0.5f);
	mCamera = SceneManager::getInstance()->mCamera;
	mStartAfter = -1;
	setLevel(MONSTER_LEVEL_2);
}

MonsterEnemy::~MonsterEnemy(void)
{
	SAFE_FREE(mArrow);
}
