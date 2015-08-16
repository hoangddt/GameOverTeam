#include "GameObject.h"
#include "LogHeaderAndroid.h"

float GameObject::mGameTime = 0.0f;
float GameObject::mFrameTime = 0.0f;
float GameObject::mLastTime = 0.0f;

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
	 for (int i = 0, j = this->mCountOfTextures; i < this->mCountOfCubetexs; i++, j++) 
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

	if (this->mpShader->mAttrNormal != -1)
	{
		glEnableVertexAttribArray(this->mpShader->mAttrNormal);
		glVertexAttribPointer(this->mpShader->mAttrNormal, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)offsetof(Vertex, normal));
	}
	
	if (this->mpShader->mUniTime != -1)
	{
		glUniform1f(this->mpShader->mUniTime, mGameTime);
		if (mGameTime > 15.0f)
		{
			mGameTime -= 15.0f;
		}
	}

	if (this->mpShader->mUniWorldMatrix != -1)
	{
		glUniformMatrix4fv(this->mpShader->mUniWorldMatrix, 1, GL_FALSE, (float *) &this->mWorldMatrix);
	}

	if (this->mpShader->mUniFinalMatrix != -1) 
	{
		glUniformMatrix4fv(this->mpShader->mUniFinalMatrix, 1, GL_FALSE, (float *) &finalMatrix);
	}
}

void GameObject::drawAndFree() 
{
	// draw triangles and bind buffers to zero
	//model type is .got
	if (this->mpModel->mCountOfMaterials > 0)
	{
		for ( int i = 0; i < this->mpModel->mCountOfMaterials; i++ )
		{
			if (this->mpShader->mUniAmbient != -1)
			{
				glUniform3f(this->mpShader->mUniAmbient,
					this->mpModel->mAmbientArr[i].x,
					this->mpModel->mAmbientArr[i].y,
					this->mpModel->mAmbientArr[i].z);
			}

			if (this->mpShader->mUniDiffuse != -1)
			{
				glUniform3f(this->mpShader->mUniDiffuse,
					this->mpModel->mDiffuseArr[i].x,
					this->mpModel->mDiffuseArr[i].y,
					this->mpModel->mDiffuseArr[i].z);
			}

			if (this->mpShader->mUniSpecular != -1)
			{
				glUniform3f(this->mpShader->mUniSpecular,
					this->mpModel->mSpecularArr[i].x,
					this->mpModel->mSpecularArr[i].y,
					this->mpModel->mSpecularArr[i].z);
			}

			if (this->mpShader->mUniTransparent != -1)
			{
				glUniform1f(this->mpShader->mUniTransparent, this->mpModel->mTransparent);
			}

			glDrawArrays(GL_TRIANGLES,
				this->mpModel->mFirstArr[i],
				this->mpModel->mCountArr[i]);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	//model type is nfg
	else 
	{
		glDrawElements(GL_TRIANGLES, this->mpModel->mCountOfIndices, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
    clock_t currentTime = clock();
	mFrameTime = (float)(currentTime - mLastTime) / CLOCKS_PER_SEC;
	mGameTime += mFrameTime;
	mLastTime = currentTime;
	this->calcWorldMatrix();
}

void GameObject::render(Camera* camera)
{
	glUseProgram(this->mpShader->mProgram);
	bindBuffers();
	useTextures();
	useCubetexs();
	configShaderFields(camera->mCameraMatrix * this->mWorldMatrix);

	//must have camera matrix to get location of this uniform
	if (this->mpShader->mUniCameraPos != -1)
	{
		glUniformMatrix4fv(this->mpShader->mUniCameraPos, 1, GL_FALSE, (float *) &camera->mCameraMatrix);
	}

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

	mScaling = Vector3(1.0f, 1.0f, 1.0f);
}

GameObject::GameObject()
{
	mScaling = Vector3(1.0f, 1.0f, 1.0f);
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
	objTmp->mIsVisible       = this->mIsVisible;

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

bool GameObject::isVisible() const
{
	return this->mIsVisible;
}

void GameObject::setVisible(bool isVisible) 
{
	this->mIsVisible = isVisible;
}

float GameObject::getActualHeight() const
{
	// Default is 1
	// Todo: Caculate exact width and height
	float defaultValue = 1.1f;
	return defaultValue * mScaling.z;
}
float GameObject::getActualWidth() const
{
	// Default is 1
	// Todo: Caculate exact width and height
	float defaultValue = 1.1f;
	return defaultValue * mScaling.x;
}

float GameObject::getActualHeightY() const
{
	float defaultValue = 1.1f;
	return defaultValue * mScaling.y;
}

bool GameObject::isInBound(float point, float min, float max)
{
	return point >= min && point <= max;
}

bool GameObject::isInBound(float objMin, float objMax, float min, float max)
{
	return isInBound(objMin, min, max) ||
		isInBound(objMax, min, max) || 
		isInBound((objMin + objMax) / 2.0f, min, max);
}

bool GameObject::impactWith(const GameObject* obj, float vPadding, float hPadding) const
{
	// impact x
	{
		float objHalfWidth = obj->getActualWidth() / 2.0f;
		float thisHalfWidth = getActualWidth() / 2.0f;

		if(!isInBound(
			obj->mPostion.x - objHalfWidth, 
			obj->mPostion.x + objHalfWidth, 
			mPostion.x - thisHalfWidth + vPadding,  
			mPostion.x + thisHalfWidth - vPadding))
		{
			return false;
		}
	}
	// impact y
	{
		float objHalfHeighY = obj->getActualHeightY() / 2.0f;
		float thisHalfHeightY = getActualHeightY() / 2.0f;

		if(!isInBound(
			obj->mPostion.y - objHalfHeighY, 
			obj->mPostion.y + objHalfHeighY, 
			mPostion.y - thisHalfHeightY + hPadding,  
			mPostion.y + thisHalfHeightY - hPadding))
		{
			return false;
		}
	}
	// impact z
	{
		float objHalfHeigh = obj->getActualHeight() / 2.0f;
		float thisHalfHeight = getActualHeight() / 2.0f;

		if(!isInBound(
			obj->mPostion.z - objHalfHeigh, 
			obj->mPostion.z + objHalfHeigh, 
			mPostion.z - thisHalfHeight + vPadding,  
			mPostion.z + thisHalfHeight - vPadding))
		{
			return false;
		}
	}
	return true;
}
