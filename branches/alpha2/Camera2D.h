#pragma once
#include "../Utilities/Utilities.h"
#include "Lib.h"

class Camera2D
{
private:
	float mHeight;
	float mWidth;
	float mRatio;
	Matrix mProjection;
	Matrix mViewMatrix;
	void calcViewMatrix();
	void calcProjectionMatrix();

	static Camera2D* mInstance;
	Camera2D(float width, float height);
	Camera2D(const Camera2D &){} 
	Camera2D& operator =(const Camera2D &){}
	~Camera2D(void);
public:
	static void createInstance(float width, float height);
	static Camera2D* getInstance();
	static void destroyInstance();
	float getWidthRatio() const;
	// use only with full screen obj
	bool isOnRect(float xReal, float yReal, const Rect& designRect);
	Matrix mCameraMatrix;
	void init();
};
