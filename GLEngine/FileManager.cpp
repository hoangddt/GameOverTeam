#include "FileManager.h"
#include "Lib.h"
#include <string.h>
#include <stdio.h>

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

const char* FileManager::combineWithRoot(const char* fileName) const
{
	int lengthOfRootPath = strlen(mRootPath),
		lengthOfRelativePath = strlen(fileName);
	
	// plus 2 because we append NULL and '/' character
	char *realPath = new char[lengthOfRootPath + lengthOfRelativePath + 2];
	strcpy(realPath, mRootPath);
	strcat(realPath, "/");
	strcat(realPath, fileName);
	return realPath;
}

void FileManager::init(const char* rootPath)
{
	int length = strlen(rootPath) + 1;
	mRootPath = new char[length];
	strcpy(mRootPath, rootPath);
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
	delete []path;
	return fp;
}
void FileManager::closeFile(FILE* fp)
{
	fclose(fp);
}