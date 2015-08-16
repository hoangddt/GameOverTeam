#pragma once
#include "../Utilities/Utilities.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include "Lib.h"

class Camera2D;

enum Visibility
{
	VISIBILITY,		// thay dc + touch dc
	INVISIBILITY,	// ko thay dc + touch dc
	COLLAPSED		// ko thay dc + ko tuoch dc
};

class Game2DObject
{
private:
	Model* mModel;
	Shader* mShader;
	Matrix mWorldMatrix;
	Vector3 mSize;
	Vector3 mPosition;
	void calcWordMatrix();
	void useTexture();
	void bindBuffers();
	void configShaderFields(const Matrix& finalMatrix);
	void drawAndFree();
public:
	Texture* mTexture;
	Visibility mVisibility;
	void setFullScreen();
	void setPosition(float designCenterX, float designCenterY);
	void setSize(float designWidth);
	void setWidth(float designWidth);
	void init();
	void update();
	void render();
	void destroy();
	Game2DObject(Model* model, Shader* shader, Texture* texture);
	virtual ~Game2DObject(void);
};
