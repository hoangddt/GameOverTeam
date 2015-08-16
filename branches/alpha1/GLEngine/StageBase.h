#pragma once

class StageBase
{
public:
	virtual void render() = 0;
	virtual void update() = 0;
	virtual void destroy() {}
	StageBase(void);
	virtual ~StageBase(void);
};
