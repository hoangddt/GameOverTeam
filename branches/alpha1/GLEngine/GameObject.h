#pragma once
#include "../Utilities/Matrix.h"
#include "../Utilities/Vector3.h"

#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Vertex.h"
#include "ResourceManager.h"

class GameObject
{
protected:
	Model* mpModel;
	Shader* mpShader;
	Texture** mpTextures;
	Texture** mpCubetexs;
	int mCountOfTextures;
	int mCountOfCubetexs;
	Matrix mWorldMatrix;
	bool mIsVisible;

	void calcWorldMatrix();
	void useTextures();
	void useCubetexs();
	void bindBuffers();// vertices and indices
	void configShaderFields(const Matrix& finalMatrix);// attributes and uniforms
	void drawAndFree();// draw triangles and bind buffers to zero
public:
	Vector3 mPostion;
	Vector3 mScaling;
	Vector3 mRotation;

	void setTextureIds(const int* textureIds, int n_textures);
	void setTexture(Texture* texture);
	virtual void init();
	virtual void update();
	virtual void render(Camera* camera);
	void destroy();
	GameObject(
		int modelId,
		int shaderId,
		const int* textureIds = NULL, 
		int n_textureIds = 0,
		const int* cubetexIds = NULL,
		int n_cubetexIds = 0);
	GameObject();
	virtual ~GameObject();
	virtual GameObject* clone() const;
	bool isVisible();
	void setVisible(bool isVisible);
	float getActualHeight();
	float getActualWidth();
};
