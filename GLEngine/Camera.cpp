#include "Camera.h"
#include "InputManager.h"
#include "../Utilities/Timer.h"
#include "../Utilities/Vector4.h"
#include "../Utilities/Utilities.h"

Camera::Camera()
{
	mSpeed = 5.0f * Timer::FrameTime();
	mNearPlane = 0.1f;
	mFarPlane = 100.0f;
	mAspect = (float) 1.0f;
	mFovy = 1.0f;

	mObject3D = true;

	mPosition = CAMERA_POSITION_FRONT;
	mRotation = CAMERA_ROTATION_FRONT;
}

void Camera::init()
{
	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	mObject3D ? calcProjection3D(): calcProjection2D();
	calcViewMatrix();
	mCameraMatrix = mProjection * mViewMatrix;
}

void Camera::update()
{
	fullMove();
	mObject3D ? calcProjection3D(): calcProjection2D();
	calcViewMatrix();
	mCameraMatrix = mProjection * mViewMatrix;
}

void Camera::calcViewMatrix()
{
	Matrix positionMatrix;
	positionMatrix.SetTranslation(-mPosition);

	Matrix rotationX;
	Matrix rotationY;
	Matrix rotationZ;
	rotationX.SetRotationX(-mRotation.x);
	rotationY.SetRotationY(-mRotation.y);
	rotationZ.SetRotationZ(-mRotation.z);

	mViewMatrix = rotationZ * rotationX * rotationY * positionMatrix;
}

void Camera::calcProjection3D()
{
	mProjection.SetPerspective(mFovy, mAspect, mNearPlane, mFarPlane);
}

void Camera::calcProjection2D()
{
	mProjection.SetOrtho(-1, 1, -1, 1, mNearPlane, mFarPlane);
}

void Camera::calcWorldMatrix() {
	Matrix positionMatrix;

	positionMatrix.SetTranslation(this->mPosition);

	Matrix rotationX, rotationY, rotationZ;
	rotationX.SetRotationX(mRotation.x);
	rotationY.SetRotationY(mRotation.y);
	rotationZ.SetRotationZ(mRotation.z);

	this->mWorldMatrix = positionMatrix * rotationY * rotationX * rotationZ;
}

void Camera::move()
{
	InputManager *input = InputManager::getInstance();
	Vector4 moveDirection;
	mSpeed = 5.0f * Timer::FrameTime();
	if (input->isKeyDown(CAMERA_MOVE_LEFT))
	{
		moveDirection += Vector4(-mSpeed, 0.0f, 0.0f, 0.0f);
	}
	if (input->isKeyDown(CAMERA_MOVE_RIGHT)) 
	{
		moveDirection += Vector4(mSpeed, 0.0f, 0.0f, 0.0f);
	}
	if (input->isKeyDown(CAMERA_MOVE_FORWARD)) 
	{
		moveDirection += Vector4(0.0f, 0.0f, -mSpeed, 0.0f);
	}
	if (input->isKeyDown(CAMERA_MOVE_BACK)) 
	{
		moveDirection += Vector4(0.0f, 0.0f, mSpeed, 0.0f);
	}
	calcWorldMatrix();
	moveDirection = mWorldMatrix * moveDirection;
	mPosition.x = mPosition.x + moveDirection.x;
	mPosition.y = mPosition.y + moveDirection.y;
	mPosition.z = mPosition.z + moveDirection.z;
}

void Camera::rotate()
{
	InputManager *input = InputManager::getInstance();
	float angle = 3.14f/90.0f * 30 * Timer::FrameTime();
	if (input->isKeyDown(CAMERA_ROTATE_LEFT)) 
	{
		mRotation.y += angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_RIGHT)) 
	{
		mRotation.y -= angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_UP)) 
	{
		mRotation.x += angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_DOWN)) 
	{
		mRotation.x -= angle;
	}
}

void Camera::rotate(float angle)
{
	InputManager *input = InputManager::getInstance();
	if (input->isKeyDown(CAMERA_ROTATE_LEFT)) 
	{
		mRotation.y += angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_RIGHT)) 
	{
		mRotation.y -= angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_UP)) 
	{
		mRotation.x += angle;
	}
	if (input->isKeyDown(CAMERA_ROTATE_DOWN)) 
	{
		mRotation.x -= angle;
	}
}

void Camera::rotate(CameraControl direction, float angle)
{
	if ( direction == CAMERA_ROTATE_RIGHT )
	{
		mRotation.y -= angle;
	}
	if ( direction == CAMERA_ROTATE_DOWN )
	{
		mRotation.x -= angle;
	}
}

void Camera::mouseRotate()
{
	InputManager* inputManager = InputManager::getInstance();
	if(!inputManager->hasTouch(TOUCH_DRAG))
	{
		return;
	}
	TouchData touchData = inputManager->getTouchData();
	rotate(CAMERA_ROTATE_RIGHT, DEGREES_TO_RADIANS(touchData.dentalX) / 5.0f);
	rotate(CAMERA_ROTATE_DOWN, DEGREES_TO_RADIANS(touchData.dentalY) / 5.0f);
	inputManager->clearTouchState();
}

void Camera::fullMove()
{
	move();
	rotate();
	mouseRotate();
	changeCamera();
}

void Camera::changeCamera()
{	
	static int checkMode = 0;
	InputManager *input = InputManager::getInstance();
	if (input->isKeyDown('T')) checkMode = 1;
	if (input->isKeyDown('R')) checkMode = 0;

	if ( checkMode )
	{
		changeViewTime(CAMERA_POSITION_FRONT, CAMERA_ROTATION_FRONT, 30);
	}
	else
	{
		changeViewTime(CAMERA_POSITION_UP, CAMERA_ROTATION_UP, 30);
	}
}

void Camera::changeView(Vector3 pos, Vector3 rot)
{
	mPosition = pos;
	mRotation = rot;
}

void Camera::changeViewTime(Vector3 pos, Vector3 rot, int bps)
{
	static Vector3 trans;
	trans.x = (pos.x - this->mPosition.x)/bps, 
	trans.y = (pos.y - this->mPosition.y)/bps, 
	trans.z = (pos.z - this->mPosition.z)/bps;
	static Vector3 angle;
	angle.x = (rot.x - this->mRotation.x)/bps, 
	angle.y = (rot.y - this->mRotation.y)/bps, 
	angle.z = (rot.z - this->mRotation.z)/bps;
	float time = 1;
	this->mPosition += trans * time;
	this->mRotation += angle * time;
}

void Camera::setObjectType(bool type)
{
	mObject3D = type;
	update();
}

bool Camera::isUp()
{
	Matrix thu;
	float alpha = 1.0f;
	Vector3 checkPosition = CAMERA_POSITION_UP;
	if ( (mPosition.x >= checkPosition.x - alpha) && (mPosition.x <= checkPosition.x + alpha)
		&& (mPosition.y >= checkPosition.y - alpha) && (mPosition.y <= checkPosition.y + alpha)
		&& (mPosition.z >= checkPosition.z - alpha) && (mPosition.z <= checkPosition.z + alpha))
	{
		return true;
	}
	return false;
}

bool Camera::isFront()
{
	float alpha = 1.0f;
	Vector3 checkPosition = CAMERA_POSITION_FRONT;
	if ( (mPosition.x >= checkPosition.x - alpha) && (mPosition.x <= checkPosition.x + alpha)
		&& (mPosition.y >= checkPosition.y - alpha) && (mPosition.y <= checkPosition.y + alpha)
		&& (mPosition.z >= checkPosition.z - alpha) && (mPosition.z <= checkPosition.z + alpha))
	{
		return true;
	}
	return false;
}

Camera::~Camera(){}