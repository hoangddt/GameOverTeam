#include "MapObject.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Map.h"
#include "LogHeaderAndroid.h"

void MapObject::render()
{
	int n_objects = mRows * mCols;
	Camera* camera = SceneManager::getInstance()->mCamera;
	for(int i = 0; i < n_objects; i++)
	{
		MapBox* obj = *(mBoxs + i);
		if(obj->isVisible())
		{
			obj->render(camera);
		}
	}
}

void MapObject::update()
{
	//bool set = false;
	MapBox* obj;
	if(mIsGameStarted && mIsSupportFalling)
	{
		if(mMap[mCurrentRowIndex][mCurrentColIndex])
		{
			obj = getCurBox();
			if(obj->getState() != MAP_BOX_DANGER)
			{
				obj->setState(MAP_BOX_DANGER);
				obj->startFallingEffect();
			}
		}
	}
	for(int i = mRows - 1; i > -1; i--)
	{
		for(int j = mCols - 1; j > -1; j--)
		{
			obj = *(mBoxs + i * mCols + j);
			if(obj->isVisible())
			{
				/*
				if(mIsGameStarted && mIsSupportFalling && !set)
				{
					
					if(mMap[i][j])
					{
						if(obj->getState() != MAP_BOX_DANGER)
						{
							obj->setState(MAP_BOX_DANGER);
							obj->startFallingEffect();
						}
						set = true;
					}
					
				}
				*/
				obj->update();
			}
		}
	}
}

void MapObject::updateObjectsPosition()
{
	float width = (*mBoxs)->getActualWidth();
	float height = (*mBoxs)->getActualHeight();

	float wDental = width / 2;
	float hDental = height / 2;
	float wMapDental = mCols * wDental;
	float hMapDental = mRows * hDental;

	//float wDental = (width / 2.0f) * (float)(1 - mRows);
	//float hDental = (height / 2.0f) * (1 - mCols);

	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			GameObject* obj = *(mBoxs + i * mCols + j);
			obj->mPostion.y = mPosition.y;
			obj->mPostion.x = mPosition.x + width * j + wDental - wMapDental;
			//obj->mPostion.x = mPosition.x + width * j + wDental;
			obj->mPostion.z = mPosition.z + height * i + hDental - hMapDental;
			//obj->mPostion.z = mPosition.z + height * i + hDental;
		}
	}
}

void MapObject::updateObjectsScaling()
{
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			GameObject* obj = *(mBoxs + i * mCols + j);
			obj->mScaling = mScaling;
		}
	}
	updateObjectsPosition();
}

void MapObject::applyNormalState()
{
	MapBoxState state;
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			MapBox* obj = *(mBoxs + i * mCols + j);
			if(i < mCurrentRowIndex)
			{
				state = MAP_BOX_NORMAL;
			}
			else
			{
				if(i == mCurrentRowIndex)
				{
					if(j == mCurrentColIndex)
					{
						state = mMap[i][j] ? MAP_BOX_PATH : MAP_BOX_FAIL;
					}
					else
					{
						state = MAP_BOX_NORMAL;
					}
				}
				else
				{
					state = mMap[i][j] ? MAP_BOX_PATH : MAP_BOX_NORMAL;
				}
			}
			obj->setState(state);
		}
	}
}

void MapObject::applyShowPathState()
{
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			MapBox* obj = *(mBoxs + i * mCols + j);
			MapBoxState state = mMap[i][j] ? MAP_BOX_PATH : MAP_BOX_NORMAL;
			obj->setState(state);
		}
	}
}

void MapObject::applyHighlightPathState()
{
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			MapBox* obj = *(mBoxs + i * mCols + j);
			if(mMap[i][j])
			{
				obj->setState(MAP_BOX_PATH);
			}
		}
	}
}

void MapObject::setState(MapState state)
{
	mState = state;
	switch(state)
	{
	case MAP_NORMAL:
		applyNormalState();
		break;
	case MAP_SHOW_PATH:
		applyShowPathState();
		break;
	case MAP_HIGHLIGHT_PATH:
		applyHighlightPathState();
		break;
	}
}

bool MapObject::isValidPosition(uint row, uint col) const
{
	bool ret = mMap[row][col] ? true : false;
	if(mIsSupportFalling)
	{
		MapBox* obj = *(mBoxs + row * mCols + col);
		return ret && !(obj->isFalling() || obj->isFell());
	}
	return ret;
}

int MapObject::isValidCurPosition() const
{
	if(mCurrentRowIndex >= mRows)
	{
		return 0;
	}
	return mMap[mCurrentRowIndex][mCurrentColIndex];
}

void MapObject::setCharacterPosition(uint row, uint col)
{
	mCurrentRowIndex = row;
	mCurrentColIndex = col;
	setState(mState);
}

void MapObject::startGame()
{
	mCurrentRowIndex--;
	setCharacterPosition(mCurrentRowIndex, mCurrentColIndex);
	mIsGameStarted = true;
}

void MapObject::setSupportFalling(bool support)
{
	mIsSupportFalling = support;
}

Vector2 MapObject::getCurrentPosition() const
{
	Vector2 vec(mCurrentRowIndex, mCurrentColIndex);
	return vec;
}

MapBox* MapObject::getBox(uint row, uint col)
{
	if(row > mRows - 1 || col > mCols - 1)
	{
		return NULL;
	}
	return *(mBoxs + row * mCols + col);
}

MapBox* MapObject::getCurBox()
{
	return getBox(mCurrentRowIndex, mCurrentColIndex);
}

uint MapObject::getRows() const
{
	return mRows;
}

uint MapObject::getCols() const
{
	return mCols;
}

MapObject::MapObject(
					 const int** map, 
					 int row, 
					 int col, 
					 Model* model,
					 Shader* shader,
					 Texture* normalTex,
					 Texture* pathTex,
					 Texture* failTex,
					 Texture* dangerTex,
					 int dangerTimeout)
{
	mIsSupportFalling = false;
	mIsGameStarted = false;

	mRows = row;
	mCols = col;
	mBoxs = new MapBox* [row * col];

	mPosition.z = -9.5;
	mPosition.x = 0.5;
	mScaling.x = mScaling.y = mScaling.z = 1;

	Map::cloneMap(map, mMap, row, col);

	mCurrentRowIndex = row;
	for(int i = 0; i < col; i++)
	{
		if(map[row - 1][i])
		{
			mCurrentColIndex = i;
			break;
		}
	}

	MapBox* orginBox = new MapBox(
		model, 
		shader,
		normalTex,
		pathTex,
		failTex, 
		dangerTex, 
		dangerTimeout);

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			MapBox* obj = dynamic_cast<MapBox *>(orginBox->clone());
			obj->setVisible(true);
			*(mBoxs + i * col + j) = obj;
		}
	}
	delete orginBox;

	updateObjectsScaling();
	setState(MAP_NORMAL);
}

MapObject::~MapObject(void)
{
	int size = mRows * mCols;
	FREE_2D_ARRAY(mBoxs, size);
	FREE_2D_ARRAY(mMap, mRows);
}
