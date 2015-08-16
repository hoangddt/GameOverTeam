#pragma once
#include "GameObject.h"
#include "ITouchListener.h"
#include "InputManager.h"

class Button: public GameObject
{
public:
	Button();
	Button(int id);
	Button(
		int modelId,
		int shaderId,
		const int* textureIds = NULL, 
		int n_textureIds = 0,
		const int* cubetexIds = NULL,
		int n_cubetexIds = 0);
	~Button();
	Button* clone() const;
	bool containCoord(int x, int y);
	void setTouchListener(ITouchListener* listener);
	void fireOnTouchEvent(int type);
private:
	int mId;
	ITouchListener *mListener;
	TouchEvent mLastTouch;
};