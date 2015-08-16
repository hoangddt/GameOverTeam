#include "Map.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "LogHeaderAndroid.h"
#include "Lib.h"

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

int* Map::genVitriDiemQueo(int row, int sodiemqueo)
{
	// tao vi tri queo
	int* tmps = new int[row];
	for (int i = 0; i < row; i++) 
	{
		tmps[i] = 0;
	}
	for (int i = 0; i < sodiemqueo; i++)
	{
		int vitriTmp = rand() % (row-1) + 1;
		while ( tmps[vitriTmp] == 1 )
		{
			vitriTmp = rand() % (row-1) + 1;
		}

		tmps[vitriTmp] = 1;
	}

	return tmps;
}

void Map::genTheRestMap(int** map, int row, int col, int startPoint, int* viTriDiemQueos)
{
	// so lan queo giong nhau
	int solanqueotrung = 0;

	for (int i = 1; i < row ; i++)
	{
		int bonus = 0; // -1 0 1

		if ( viTriDiemQueos[i] == 1 ) 
		{
			if ( startPoint == 0 ) // nam goc trai -> queo phai
			{
				bonus = 1;
			}
			else 
			{
				if ( startPoint == col - 1 ) 
				{
					bonus = -1; // nam goc phai -> queo trai
				}
				else // nam o giua -> random queo
				{
					int tmp = ( rand() % 10 ) + 1;
					bonus = ( tmp % 2 == 0 ) ? 1 : -1;

					// kiem tra vi tri queo co trung nhau khong
					if ( (i - 2) >= 0 && map[i-2][startPoint + bonus] == 1)
					{
						solanqueotrung += 1;
					}

					if ( solanqueotrung > 1 ) 
					{
						solanqueotrung = 0;
						bonus = -bonus;
					}
				}
			}
		}
		else 
		{
			bonus = 0;
		}
		
		int index = startPoint + bonus;

		map[i][index] = 1;
		startPoint = index;
	}
}

int** Map::genMap(int row, int col, GameMode mode)
{
	// Allocate Memory
	int** tempMap = new int*[row];
	for (int i = 0; i < row; i++)
		tempMap[i] =  new int[col];
	// Make map all 0
	eraseMap(tempMap, row, col);
	// generate map

	// init the first point
	// int startPoint = rand() % col;
	int startPoint = col / 2;
	tempMap[0][startPoint] = 1;

	// generate the remaining
	int* viTriDiemQueos = NULL;
	switch ( mode )
	{
		case EASY: 
			if (row > 5)
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, 1);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			else
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, 2);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			break;
		case MEDIUM:
			if (row > 5)
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, 3);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			else
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, 2);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			break;
		case HARD: 
			if (row > 5)
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, 5);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			else
			{
				// vi tri diem queo
				viTriDiemQueos = genVitriDiemQueo(row, row-2);
				genTheRestMap(tempMap, row, col, startPoint, viTriDiemQueos);
			}
			break;
	}
	SAFE_FREE(viTriDiemQueos);
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