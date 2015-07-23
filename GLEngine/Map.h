#pragma once

class Map
{
private:
	Map();
	~Map();
	Map(const Map&) {}
	Map& operator =(const Map&) {}
	static Map * mpInstance;
public:
	static Map * getInstance();
	static void destroyInstance();

	static void cloneMap(const int** src, int**& des, int row, int col);

	int** genMap(int row, int col);
	void eraseMap(int** map, int row, int col);
	void freeMap(int** &map, int row, int col);
};