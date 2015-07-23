#pragma once
#include "GameObject.h"
#include "Lib.h"

enum MapState
{
	MAP_NORMAL,
	MAP_SHOW_PATH
};

class MapObject
{
private:
	GameObject** mpObjects;
	int** mMap;
	// get from orginObject which will have 3 textures
	Texture* mNormalTex;
	Texture* mPathTex;
	Texture* mFailTex;

	uint mRows;
	uint mCols;
	uint mCurrentRow;
	uint mCurrentCol;
	MapState mState;
	
	void applyNormalState();
	void applyShowPathState();
public:
	void updateObjectsPosition();
	void updateObjectsScaling();
	void render();
	void update();
	Vector3 mPosition;
	Vector3 mScaling;
	void setState(MapState state);
	void setCharacterPosition(uint row, uint col);
	bool isValidPosition(uint row, uint col) const;
	int isValidCurPosition() const;// 1 - valid; -1 - invalid; 0 - none
	void startGame();
	Vector2 getCurrentPosition() const;
	MapObject(
		const int** map, 
		int row, 
		int col, 
		const GameObject* orginObject);
	~MapObject(void);
};
