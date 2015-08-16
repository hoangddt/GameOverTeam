#pragma once
#include "GameObject.h"
#include "MapBox.h"
#include "Lib.h"

enum MapState
{
	MAP_NORMAL,
	MAP_SHOW_PATH,
	MAP_HIGHLIGHT_PATH
};

class MapObject
{
private:
	MapBox** mBoxs;
	int** mMap;

	uint mRows;
	uint mCols;
	uint mCurrentRowIndex;
	uint mCurrentColIndex;
	MapState mState;

	bool mIsSupportFalling;
	bool mIsGameStarted;
	
	void applyNormalState();
	void applyShowPathState();
	void applyHighlightPathState();
public:
	uint getRows() const;
	uint getCols() const;
	void updateObjectsPosition();
	void updateObjectsScaling();
	void render();
	void update();
	Vector3 mPosition;
	Vector3 mScaling;
	MapBox* getBox(uint row, uint col);
	MapBox* getCurBox();
	void setState(MapState state);
	void setCharacterPosition(uint row, uint col);
	bool isValidPosition(uint row, uint col) const;
	int isValidCurPosition() const;// 1 - valid; -1 - invalid; 0 - none
	void startGame();
	Vector2 getCurrentPosition() const;
	void setSupportFalling(bool support);
	MapObject(
		const int** map, 
		int row, 
		int col, 
		Model* model,
		Shader* shader,
		Texture* normalTex,
		Texture* pathTex,
		Texture* failTex,
		Texture* dangerTex,
		int dangerTimeout);
	~MapObject(void);
};
