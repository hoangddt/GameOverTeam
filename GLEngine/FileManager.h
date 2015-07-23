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
	const char* combineWithRoot(const char* fileName) const;
	void init(const char* rootPath);
	FILE* openFile(const char* fileName, const char* mode);
	void closeFile(FILE *fp);
};
