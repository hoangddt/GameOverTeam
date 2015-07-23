#pragma once
#include "../Utilities/Matrix.h"
#include "../Utilities/Vector3.h"

#define CAMERA_POSITION_UP Vector3(0.43f, 11.0f, 0.2f)
#define CAMERA_ROTATION_UP Vector3(-1.55f, 0.0f, 0.0f)
#define CAMERA_POSITION_FRONT Vector3(0.5f, 5.7f, 9.2f)
#define CAMERA_ROTATION_FRONT Vector3(-0.63f, 0.0f, 0.0f)

enum CameraControl
{
	CAMERA_MOVE_LEFT	= 'A',
	CAMERA_MOVE_RIGHT	= 'D',
	CAMERA_MOVE_BACK	= 'S',
	CAMERA_MOVE_FORWARD	= 'W',
	CAMERA_ROTATE_UP	= 38,
	CAMERA_ROTATE_DOWN	= 40,
	CAMERA_ROTATE_LEFT	= 37,
	CAMERA_ROTATE_RIGHT = 39
};

class Camera
{
private:
	void move();// di chuyển camera
	void rotate();// xoay camera với angle mặt định
	void rotate(float angle);// xoay camera với angle
	void rotate(CameraControl direction, float angle); // xoay camera hướng direction với angle (dùng cho mouse)
	void mouseRotate(); //xoay camera bằng chuột
	void keyControl();// nhận điều khiển từ phím
	void mouseControl();// nhận điều khiển từ chuột/touch
	void changeView(Vector3 pos, Vector3 rot); //thay đổi góc nhìn của Camera
	void changeViewTime(Vector3 pos, Vector3 rot, int bps); //thay đổi góc nhìn Camera theo thời gian	
	void calcWorldMatrix();
	void calcViewMatrix();
	void calcProjection3D();
	void calcProjection2D();
	Vector3 mRotation;
	Vector3 mPosition;
	Matrix mProjection;
	Matrix mViewMatrix;
	Matrix mWorldMatrix;
	bool mObject3D;
public:
	Matrix mCameraMatrix;
	float mSpeed;
	float mNearPlane;
	float mFarPlane;
	float mAspect;
	float mFovy;
	void changeCamera();
	void setObjectType(bool type); // set type của Object 3D hoặc 2D
	bool isUp();
	bool isFront();
	virtual void update();
	virtual void init();
	void fullMove(); // điều khiển các hàm move và rotate
	Camera();
	~Camera();
};
