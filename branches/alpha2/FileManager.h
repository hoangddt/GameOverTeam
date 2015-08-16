#pragma once
#include <stdio.h>

class FileManager
{
private:
	static FileManager* mpInstance;
	char* mRootPath;
	FileManager(void){}
	FileManager(const FileManager &){}
	FileManager& operator =(const FileManager &){}
	~FileManager(void);
public:
	static FileManager* getInstance();
	static void destroyInstance();
	static const char* combine(const char* path1, const char* path2);
	const char* combineWithRoot(const char* fileName) const;
	void init(const char* rootPath);
	FILE* openFile(const char* fileName, const char* mode);
	void closeFile(FILE *fp);
};
