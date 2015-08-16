#pragma once
#include "ArrowObject.h"
#include "SceneManager.h"

enum MonsterLevel
{
	MONSTER_LEVEL_0,
	MONSTER_LEVEL_1,
	MONSTER_LEVEL_2
};

class MonsterEnemy
{
private:
	Camera* mCamera;
	MonsterLevel mLevel;
	Vector3 mStartLocation;
	int mStartAfter;
	ArrowLocation mLocation;
	long mStartTime;
	void setLevel(MonsterLevel level);
public:
	ArrowObject* mArrow;
	bool impacted(const GameObject* character) const;
	bool isVisible() const;
	void setStartAfter(int miliseconds);
	void startAt(const Vector3& pos, ArrowLocation location = ARROW_LEFT);
	void setConfig(int level, const Vector3& rotation);
	MonsterEnemy(Model* model, 
				 Shader* shader, 
				 Texture* texture);
	void render();
	void update();
	~MonsterEnemy(void);
};
