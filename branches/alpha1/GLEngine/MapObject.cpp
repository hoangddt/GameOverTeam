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
		MapBox* obj = *(mBoxs + i);
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
		MapBox* obj = *(mBoxs + i);
		if(obj->isVisible())
		{
			obj->update();
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
			if(i < mCurrentRow)
			{
				state = MAP_BOX_NORMAL;
			}
			else
			{
				if(i == mCurrentRow)
				{
					if(j == mCurrentCol)
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

MapBox* MapObject::getBox(uint row, uint col)
{
	return *(mBoxs + row * mCols + col);
}

MapBox* MapObject::getCurBox()
{
	return getBox(mCurrentRow, mCurrentCol);
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
	mRows = row;
	mCols = col;
	mBoxs = new MapBox* [row * col];

	mPosition.z = -10;
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

	MapBox* orginBox = new MapBox(model, shader, normalTex, pathTex, failTex, dangerTex);

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