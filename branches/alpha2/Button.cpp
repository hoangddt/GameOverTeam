#include "Button.h"
#include "SceneManager.h"
#include "LogHeaderAndroid.h"

Button::Button()
{
	mListener = NULL;
	mLastTouch = TOUCH_NONE;
}

Button::Button(int id)
:GameObject()
{
	mId = id;
	mListener = NULL;
	mLastTouch = TOUCH_NONE;
}

Button::Button(int modelId, int shaderId, const int* textureIds, int n_textureIds, const int* cubetexIds, int n_cubetexIds) 
: GameObject(modelId, shaderId, textureIds, n_textureIds, cubetexIds, n_cubetexIds) 
{
	mListener = NULL;
	mLastTouch = TOUCH_NONE;
}


Button::~Button()
{
	delete mListener;
}

Button* Button::clone() const
{
	Button* btnTmp = new Button;

	btnTmp->mpModel			 = this->mpModel;
	btnTmp->mpShader		 = this->mpShader;
	btnTmp->mCountOfTextures = this->mCountOfTextures;
	btnTmp->mCountOfCubetexs = this->mCountOfCubetexs;

	btnTmp->mpTextures		 = new Texture*[this->mCountOfTextures];
	for (int i = 0; i < this->mCountOfTextures; i++) 
	{
		btnTmp->mpTextures[i] = this->mpTextures[i];
	}

	btnTmp->mpCubetexs		 = new Texture*[this->mCountOfCubetexs];
	for (int i = 0; i < this->mCountOfCubetexs; i++) 
	{
		btnTmp->mpCubetexs[i] = this->mpCubetexs[i];
	}

	btnTmp->mWorldMatrix	 = this->mWorldMatrix;
	btnTmp->mPostion		 = this->mPostion;
	btnTmp->mScaling		 = this->mScaling;
	btnTmp->mRotation		 = this->mRotation;

	return btnTmp;
}
bool Button::containCoord(int x, int y)
{
	float width = getActualWidth(),
		  height = getActualHeight();
	float rangeX = mPostion.x + width,
		  rangeY = mPostion.y + height;

	if (x >= mPostion.x && x <= rangeX)
	{
		if (y >= mPostion.y && y <= rangeY)
		{
			return true;
		}
	}
	return false;
}

void Button::setTouchListener(ITouchListener* listener)
{
	mListener = listener;
}

void Button::fireOnTouchEvent(int type)
{
	if (mListener != NULL && mLastTouch == TOUCH_DOWN && type == TOUCH_UP)
	{
		mListener->touchPerformed(mId);
		mLastTouch = TOUCH_NONE;
	}
	else
	{
		mLastTouch = (TouchEvent)type;
	}
}