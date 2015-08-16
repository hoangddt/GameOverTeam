#include "FileManager.h"
#include "Lib.h"
#include <string.h>
#include <stdio.h>
#include "LogHeaderAndroid.h"

FileManager* FileManager::mpInstance = NULL;

FileManager* FileManager::getInstance()
{
	if (mpInstance == NULL)
		mpInstance = new FileManager;
	return mpInstance;
}
void FileManager::destroyInstance()
{
	SAFE_FREE(mpInstance);
}
FileManager::~FileManager(void)
{
	delete []mRootPath;
}

const char* FileManager::combine(const char* path1, const char* path2)
{
	int lengthOfRootPath = strlen(path1),
		lengthOfRelativePath = strlen(path2);
	
	// plus 2 because we append NULL and '/' character
	char *realPath = new char[lengthOfRootPath + lengthOfRelativePath + 2];
	strcpy(realPath, path1);
	strcat(realPath, "/");
	strcat(realPath, path2);
	return realPath;
}

const char* FileManager::combineWithRoot(const char* fileName) const
{
	return combine(mRootPath, fileName);
}

void FileManager::init(const char* rootPath)
{
	int length = strlen(rootPath) + 1;
	mRootPath = new char[length];
	strcpy(mRootPath, rootPath);
	printf("FileManager class. Just set rootPath: %s\n", mRootPath);
}
FILE* FileManager::openFile(const char* fileName, const char* mode)
{
	const char* path = combineWithRoot(fileName);
	FILE *fp = fopen(path, mode);
	if (fp == NULL)
	{
		printf("Error when reading file: %s\n", path);
		return NULL;
	}
	printf("FileManager class. Openning file: %s\n", path);
	delete []path;
	return fp;
}
void FileManager::closeFile(FILE* fp)
{
	fclose(fp);
}