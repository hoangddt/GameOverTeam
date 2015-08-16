#pragma once
#include "StageBase.h"

class GameMapStage: public StageBase
{
public:
	bool isStage1Press() const;
	bool isStage2Press() const;
	bool isStage3Press() const;
};