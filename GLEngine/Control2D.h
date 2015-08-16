#pragma once
#include "Game2DObject.h"

#define MENU_MAX_BUTTONS_SUPPORT 5

class Control2D
{
private:
	const char* mPath;
	Rect* mPosition;
	int mNumOfElements;
	bool mButtonsDown[MENU_MAX_BUTTONS_SUPPORT];
	int mLastButtonIdDown;
	void initialize();
protected:
	Control2D();
	bool mEnable;
public:
	Game2DObject* mObject;
	Control2D(int modelId, int shaderId, int textureId, int pathId);
	void setEnable(bool enable);
	void setTexture(Texture* texture);
	void init(const char *path);
	void init();
	virtual void render();
	void update();
	bool isButtonPress(int id);
	void setVisible(Visibility);
	bool getIsVisible() const;
	virtual ~Control2D();
};