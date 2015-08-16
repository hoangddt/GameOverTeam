#pragma once
#include "StageBase.h"

#define RES_MAPSTAGE1_ID	0
#define RES_MAPSTAGE2_ID	1
#define RES_MAPSTAGE3_ID	2
#define RES_MAPSTAGE4_ID	3

class GameMapStage: public StageBase
{
public:
	virtual void init(const char* rmFile, 
					  const char* smFile, 
					  const char* bgSoundId);
	bool isStage1Press() const;
	bool isStage2Press() const;
	bool isStage3Press() const;
	GameMapStage(void);
};