#include "Game2DObject.h"
#include "Camera2D.h"

void Game2DObject::setFullScreen()
{
	mPosition.x = 0.0f;
	mPosition.y = 0.0f;

	Camera2D* camera = Camera2D::getInstance();
	mSize.x = DESIGN_SCREEN_WIDTH * camera->getWidthRatio() / 2.0f + 1.0f;
	mSize.y = DESIGN_SCREEN_HEIGHT * camera->getWidthRatio() / 2.0f + 1.0f;
	mSize.z = 0.0f;
}

Vector2 Game2DObject::getDesignPosition() const
{
	return mDesignPosition;
}

void Game2DObject::setPosition(float designCenterX, float designCenterY)
{
	mDesignPosition.x = designCenterX;
	mDesignPosition.y = designCenterY;

	Camera2D* camera = Camera2D::getInstance();
	mPosition.x = designCenterX * camera->getWidthRatio();
	mPosition.y = designCenterY * camera->getWidthRatio();

	calcWordMatrix();
}

void Game2DObject::setSize(float designWidth, float designHeight)
{
	designWidth /= 2.0f;
	designHeight /= 2.0f;

	float ratio = Camera2D::getInstance()->getWidthRatio();

	mSize.x = designWidth * ratio;
	mSize.y = designHeight * ratio;

	calcWordMatrix();
}

void Game2DObject::setSize(float designWidth)
{
	setWidth(designWidth);
	mSize.y = mSize.x;
	calcWordMatrix();
}

void Game2DObject::setWidth(float designWidth)
{
	designWidth /= 2.0f;
	mSize.x = designWidth * Camera2D::getInstance()->getWidthRatio();
	calcWordMatrix();
}

void Game2DObject::calcWordMatrix()
{
	Matrix m_postion, m_size;
	m_postion.SetTranslation(mPosition);
	m_size.SetScale(mSize);
	mWorldMatrix = m_postion * m_size;
}

void Game2DObject::useTexture()
{
	if(mShader->mUniTextures[0] != -1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture->mTextureId);
		glUniform1i(mShader->mUniTextures[0], 0);
	}
}

void Game2DObject::bindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, mModel->vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mModel->ibo);
}

void Game2DObject::configShaderFields(const Matrix& finalMatrix)
{
	if(mShader->mAttrUV != -1)
	{
		glEnableVertexAttribArray(mShader->mAttrUV);
		glVertexAttribPointer(mShader->mAttrUV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	}

	if(mShader->mAttrPosition != -1)
	{
		glEnableVertexAttribArray(mShader->mAttrPosition);
		glVertexAttribPointer(mShader->mAttrPosition, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
	}

	if(mShader->mUniFinalMatrix != -1)
	{
		glUniformMatrix4fv(mShader->mUniFinalMatrix, 1, GL_FALSE, (float *)&finalMatrix);
	}
}

void Game2DObject::drawAndFree()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	glDrawElements(GL_TRIANGLES, mModel->mCountOfIndices, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}

void Game2DObject::init()
{
	calcWordMatrix();
}

void Game2DObject::update()
{
	if(mVisibility != COLLAPSED)
	{
		calcWordMatrix();
	}
}

void Game2DObject::render()
{
	if(mVisibility == VISIBILITY)
	{
		glUseProgram(mShader->mProgram);
		bindBuffers();
		useTexture();
		configShaderFields(Camera2D::getInstance()->mCameraMatrix * mWorldMatrix);
		mShader->enableState();
		drawAndFree();
		mShader->disableState();
	}
}

void Game2DObject::destroy()
{
}

Game2DObject::Game2DObject(Model* model, Shader* shader, Texture* texture)
{
	mModel = model;
	mShader = shader;
	mTexture = texture;
	mVisibility = VISIBILITY;
}

Game2DObject::~Game2DObject(void)
{
	destroy();
}
