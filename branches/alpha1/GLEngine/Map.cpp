#include "Map.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


Map * Map::mpInstance = NULL;
Map * Map::getInstance()
{
	if (mpInstance==NULL)
		mpInstance = new Map();
	return mpInstance;
}
void Map::destroyInstance()
{
	delete mpInstance;
	mpInstance = NULL;
}

void Map::cloneMap(const int** src, int**& des, int row, int col)
{
	des = new int* [row];
	for (int i = 0; i < row; i++)
	{
		des[i] =  new int[col];
	}
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < col; j++)
		{
			des[i][j] = src[i][j];
		}
	}
}

Map::Map()
{
	srand( (unsigned int) time(NULL));
}

Map::~Map()
{

}

int** Map::genMap(int row, int col)
{
	// Allocate Memory
	int** tempMap = new int*[row];
	for (int i = 0; i < row; i++)
		tempMap[i] =  new int[col];
	// Make map all 0
	eraseMap(tempMap, row, col);
	// generate map

	// init the first point
	int startPoint = rand() % col;
	tempMap[0][startPoint] = 1;
	// generate the remaining
	for (int i = 1; i < row ; i++)
	{
		int bonus = rand() % 3 - 1;   // -1 0 1
		int index = startPoint + bonus;
		index = index < 0 ? 0 : index;
		index = index > (col-1) ? (col-1) : index;

		tempMap[i][index] = 1;
		startPoint = index;
	}
	
	return tempMap;
}
void Map::eraseMap(int** map, int row, int col)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < col; j++)
			map[i][j] = 0;
}
// free memory
void Map::freeMap(int** &map, int row, int col)
{
	for (int i = 0; i < row; i++)
		delete map[i];
	delete []map;
	map = NULL;
}