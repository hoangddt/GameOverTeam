#include "GameObject.h"

void GameObject::calcWorldMatrix() 
{
	Matrix positionMatrix;
	Matrix scalingMatrix;

	positionMatrix.SetTranslation(this->mPostion);

	scalingMatrix.SetScale(this->mScaling);

	Matrix rotationX, rotationY, rotationZ;
	rotationX.SetRotationX(mRotation.x);
	rotationY.SetRotationY(mRotation.y);
	rotationZ.SetRotationZ(mRotation.z);

	this->mWorldMatrix = positionMatrix * rotationY * rotationX * rotationZ * scalingMatrix;
}

void GameObject::useTextures() 
{
	for (int i = 0; i < this->mCountOfTextures; i++) 
	{
		if (this->mpShader->mUniTextures[i] != -1) 
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, this->mpTextures[i]->mTextureId);
			glUniform1i(this->mpShader->mUniTextures[i], i);
		}
	}
}

void GameObject::useCubetexs() 
{
	 for (int i = 0, j = this->mCountOfCubetexs; i < this->mCountOfCubetexs; i++, j++) 
	 {
		 if (this->mpShader->mUniTextures[j] != -1) 
		 {
			 glActiveTexture(GL_TEXTURE0 + j);
			 glBindTexture(GL_TEXTURE_CUBE_MAP, this->mpCubetexs[i]->mTextureId);
			 glUniform1i(this->mpShader->mUniTextures[j], j);
		 }
	 }
}

void GameObject::bindBuffers() 
{// vertices and indices
	glBindBuffer(GL_ARRAY_BUFFER, this->mpModel->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mpModel->ibo);
}

void GameObject::configShaderFields(const Matrix& finalMatrix) 
{// attributes and uniforms
	if (this->mpShader->mAttrUV != -1) 
	{
		glEnableVertexAttribArray(this->mpShader->mAttrUV);
		glVertexAttribPointer(this->mpShader->mAttrUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	}

	if (this->mpShader->mAttrPosition != -1) 
	{
		glEnableVertexAttribArray(this->mpShader->mAttrPosition);
		glVertexAttribPointer(this->mpShader->mAttrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
	}

	if (this->mpShader->mUniFinalMatrix != -1) 
	{
		glUniformMatrix4fv(this->mpShader->mUniFinalMatrix, 1, GL_FALSE, (float *)&finalMatrix);
	}
}

void GameObject::drawAndFree() 
{// draw triangles and bind buffers to zero
	glDrawElements(GL_TRIANGLES, this->mpModel->mCountOfIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GameObject::setTexture(Texture* texture)
{
	if (this->mpTextures != NULL) 
	{
		delete[] mpTextures;
	}
	mpTextures = new Texture* [1];
	mpTextures[0] = texture;
}

void GameObject::setTextureIds(const int* textureIds, int n_textures) 
{
	ResourceManager* resManager = ResourceManager::getInstance();

	if (this->mpTextures != NULL) 
	{
		delete[] this->mpTextures;
	}

	this->mCountOfTextures = n_textures;
	this->mpTextures = new Texture*[n_textures];

	for (int i = 0; i < n_textures; i++)
	{
		this->mpTextures[i] = resManager->getTextureById(textureIds[i]);
	}
}

void GameObject::init() 
{
	this->calcWorldMatrix();
}

void GameObject::update() 
{
	this->calcWorldMatrix();
}

void GameObject::render(Camera* camera)
{
	glUseProgram(this->mpShader->mProgram);
	bindBuffers();
	useTextures();
	useCubetexs();
	configShaderFields(camera->mCameraMatrix * this->mWorldMatrix);
	this->mpShader->enableState();
	drawAndFree();
	this->mpShader->disableState();
}

void GameObject::destroy() 
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	FREE_1D_ARRAY(this->mpTextures);
	FREE_1D_ARRAY(this->mpCubetexs);
}

GameObject::GameObject(int modelId, int shaderId, const int* textureIds, int n_textureIds, const int* cubetexIds, int n_cubetexIds) 
{
	ResourceManager* resManager = ResourceManager::getInstance();
	
	if (modelId > -1) 
	{
		this->mpModel = resManager->getModelById(modelId);
	}

	if (shaderId > -1) 
	{
		this->mpShader = resManager->getShaderById(shaderId);
	}

	if (textureIds != NULL) 
	{
		this->mCountOfTextures = n_textureIds;
		this->mpTextures = new Texture*[n_textureIds];

		for (int i = 0; i < n_textureIds; i++)
		{
			this->mpTextures[i] = resManager->getTextureById(textureIds[i]);
		}
	}

	if (cubetexIds != NULL) 
	{
		this->mCountOfCubetexs = n_cubetexIds;
		this->mpCubetexs = new Texture*[n_cubetexIds];

		for (int i = 0; i < n_cubetexIds; i++) 
		{
			this->mpCubetexs[i] = resManager->getCubetexById(cubetexIds[i]);
		}
	}
}

GameObject::GameObject()
{
}

GameObject::~GameObject() 
{
	this->destroy();
}

GameObject* GameObject::clone() const
{
	GameObject* objTmp = new GameObject;

	objTmp->mpModel			 = this->mpModel;
	objTmp->mpShader		 = this->mpShader;
	objTmp->mCountOfTextures = this->mCountOfTextures;
	objTmp->mCountOfCubetexs = this->mCountOfCubetexs;

	objTmp->mpTextures		 = new Texture*[this->mCountOfTextures];
	for (int i = 0; i < this->mCountOfTextures; i++) 
	{
		objTmp->mpTextures[i] = this->mpTextures[i];
	}

	objTmp->mpCubetexs		 = new Texture*[this->mCountOfCubetexs];
	for (int i = 0; i < this->mCountOfCubetexs; i++) 
	{
		objTmp->mpCubetexs[i] = this->mpCubetexs[i];
	}

	objTmp->mWorldMatrix	 = this->mWorldMatrix;
	objTmp->mPostion		 = this->mPostion;
	objTmp->mScaling		 = this->mScaling;
	objTmp->mRotation		 = this->mRotation;

	return objTmp;
}

bool GameObject::isVisible() 
{
	return this->mIsVisible;
}

void GameObject::setVisible(bool isVisible) 
{
	this->mIsVisible = isVisible;
}

float GameObject::getActualHeight()
{
	// Default is 1
	// Todo: Caculate exact width and height
	float defaultValue = 1.1;
	return defaultValue * mScaling.z;
}
float GameObject::getActualWidth()
{
	// Default is 1
	// Todo: Caculate exact width and height
	float defaultValue = 1.1;
	return defaultValue * mScaling.x;
}