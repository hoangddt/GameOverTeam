#include "Camera2D.h"
#include "Lib.h"

Camera2D* Camera2D::mInstance = NULL;

void Camera2D::createInstance(float width, float height)
{
	destroyInstance();
	mInstance = new Camera2D(width, height);
	mInstance->init();
}

Camera2D* Camera2D::getInstance()
{
	return mInstance;
}

void Camera2D::destroyInstance()
{
	SAFE_FREE(mInstance);
}

void Camera2D::calcViewMatrix()
{
	mViewMatrix.SetOrtho(
		-mWidth / 2.0f, mWidth / 2.0f,
		-mHeight / 2.0f, mHeight / 2.0f, 
		0.1f, 100.0f);
}

void Camera2D::calcProjectionMatrix()
{
	mProjection.SetPerspective(DEGREES_TO_RADIANS(90.0f), 1.0f, 0.1f, 100.0f);
}

void Camera2D::init()
{
	calcProjectionMatrix();
	calcViewMatrix();
	mCameraMatrix = mProjection * mViewMatrix;
}

float Camera2D::getWidthRatio() const
{
	return mRatio;
}

bool Camera2D::isOnRect(float xReal, float yReal, const Rect& designRect)
{
	float xStd = xReal / mRatio;
	float yStd = yReal / mRatio;
	float paddingTop = (mHeight - DESIGN_SCREEN_HEIGHT * mRatio) / 2.0f;
	paddingTop = paddingTop / mRatio;
	return (xStd >= designRect.left && xStd <= designRect.right) &&
		(yStd >= designRect.top + paddingTop && yStd <= designRect.bottom + paddingTop);
}

Camera2D::Camera2D(float width, float height)
{
	mWidth = width;
	mHeight = height;
	mRatio = width / DESIGN_SCREEN_WIDTH;
}

Camera2D::~Camera2D(void)
{
}
