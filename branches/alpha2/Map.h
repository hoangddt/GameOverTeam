#pragma once

enum GameMode 
{
	EASY,
	MEDIUM,
	HARD
};

class Map
{
private:
	Map();
	~Map();
	Map(const Map&) {}
	Map& operator =(const Map&) {}
	static Map * mpInstance;

	int* genVitriDiemQueo(int row, int sodiemqueo);
	void genTheRestMap(int** map, int row, int col, int startPoint, int* viTriDiemQueos);

public:
	static Map * getInstance();
	static void destroyInstance();

	static void cloneMap(const int** src, int**& des, int row, int col);

	int** genMap(int row, int col, GameMode mode);
	void eraseMap(int** map, int row, int col);
	void freeMap(int** &map, int row, int col);
};