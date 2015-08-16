#pragma once
#include "GamePlayingStage2.h"
#include "EnemyManager.h"

#define RES_DEMON_ID		8
#define RES_BALL_MODEL_ID	11
#define RES_BALL_SHADER_ID	0
#define RES_BALL_TEXTURE_ID	14

class GamePlayingStage3: public GamePlayingStage2
{
protected:
	EnemyManager* mEnemyManager;
	bool mIsJumping;
	bool mIsJumpEnd;
	virtual void move(Direction direction);
	virtual void checkNewState();
	virtual void setLevel(int level);
	virtual void ready();
	void jump(bool init);
public:
	virtual void start();
	virtual void update();
	virtual void render();
	virtual void destroy();
	GamePlayingStage3(void);
	~GamePlayingStage3(void);
};
