#include "MapObject.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Map.h"

void MapObject::render()
{
	int n_objects = mRows * mCols;
	Camera* camera = SceneManager::getInstance()->mCamera;
	for(int i = 0; i < n_objects; i++)
	{
		GameObject* obj = *(mpObjects + i);
		if(obj->isVisible())
		{
			obj->render(camera);
		}
	}
}

void MapObject::update()
{
	int n_objects = mRows * mCols;
	for(int i = 0; i < n_objects; i++)
	{
		GameObject* obj = *(mpObjects + i);
		if(obj->isVisible())
		{
			obj->update();
		}
	}
}

void MapObject::updateObjectsPosition()
{
	float width = (*mpObjects)->getActualWidth();
	float height = (*mpObjects)->getActualHeight();
	
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
			GameObject* obj = *(mpObjects + i * mCols + j);
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
			GameObject* obj = *(mpObjects + i * mCols + j);
			obj->mScaling = mScaling;
		}
	}
	updateObjectsPosition();
}

void MapObject::applyNormalState()
{
	Texture* texutre;
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			GameObject* obj = *(mpObjects + i * mCols + j);
			if(i < mCurrentRow)
			{
				texutre = mNormalTex;
			}
			else
			{
				if(i == mCurrentRow)
				{
					if(j == mCurrentCol)
					{
						texutre = mMap[i][j] ? mPathTex : mFailTex;
					}
					else
					{
						texutre = mNormalTex;
					}
				}
				else
				{
					texutre = mMap[i][j] ? mPathTex : mNormalTex;
				}
			}
			obj->setTexture(texutre);
		}
	}
}

void MapObject::applyShowPathState()
{
	for(int i = 0; i < mRows; i++)
	{
		for(int j = 0; j < mCols; j++)
		{
			GameObject* obj = *(mpObjects + i * mCols + j);
			Texture *texture = mMap[i][j] ? mPathTex : mNormalTex;
			obj->setTexture(texture);
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
	}
}

bool MapObject::isValidPosition(uint row, uint col) const
{
	return mMap[row][col] ? true : false;
}

int MapObject::isValidCurPosition() const
{
	if(mCurrentRow >= mRows)
	{
		return 0;
	}
	return mMap[mCurrentRow][mCurrentCol];
}

void MapObject::setCharacterPosition(uint row, uint col)
{
	mCurrentRow = row;
	mCurrentCol = col;
	setState(mState);
}

void MapObject::startGame()
{
	mCurrentRow--;
	setCharacterPosition(mCurrentRow, mCurrentCol);
}

Vector2 MapObject::getCurrentPosition() const
{
	Vector2 vec(mCurrentRow, mCurrentCol);
	return vec;
}

MapObject::MapObject(const int** map, int row, int col, const GameObject* orginObject)
{
	mRows = row;
	mCols = col;
	mpObjects = new GameObject* [row * col];
	mScaling.x = mScaling.y = mScaling.z = 1;

	Map::cloneMap(map, mMap, row, col);
	
	mCurrentRow = row;
	for(int i = 0; i < col; i++)
	{
		if(map[row - 1][i])
		{
			mCurrentCol = i;
			break;
		}
	}

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			GameObject* obj = orginObject->clone();
			obj->setVisible(true);
			*(mpObjects + i * col + j) = obj;
		}
	}
	updateObjectsScaling();
	ResourceManager* res = ResourceManager::getInstance();
	mNormalTex = res->getTextureById(0);
	mPathTex = res->getTextureById(1);
	mFailTex = res->getTextureById(2);
	setState(MAP_NORMAL);
}

MapObject::~MapObject(void)
{
	int size = mRows * mCols;
	FREE_2D_ARRAY(mpObjects, size);
	FREE_2D_ARRAY(mMap, mRows);
}