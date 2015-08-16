#include "SceneManager.h"
#include "FileManager.h"
#include "Texture.h"
#include "Lib.h"
#include "ControlButton.h"
#include "LogHeaderAndroid.h"

SceneManager * SceneManager::mInstance = NULL;

SceneManager * SceneManager::getInstance() 
{
	if ( mInstance == NULL )
			mInstance = new SceneManager;
	return mInstance;
}

void SceneManager::destroyInstance() 
{
	if ( mInstance )
	{
		delete mInstance;
		mInstance = NULL;
	}
}

SceneManager::SceneManager()
{
	mCamera = NULL;
	mObjects = NULL;
	mCountOfObjects = 0;
}

void SceneManager::initCamera(FILE *ptr_f)
{
	mCamera = new Camera;
	fscanf(ptr_f, "#CAMERA\n");
	fscanf(ptr_f, "NEAR %f", &mCamera->mNearPlane);
	fscanf(ptr_f, "FAR %f", &mCamera->mFarPlane);
	fscanf(ptr_f, "FOV %f", &mCamera->mFovy);
	fscanf(ptr_f, "SPEED %f", &mCamera->mSpeed);
	mCamera->init();
	mCamera->setObjectType(true);
}

void SceneManager::initObjects(FILE *ptr_f)
{
	mCountOfObjects = 0;
	fscanf(ptr_f, "#Objects: %d\n", &mCountOfObjects);
	mObjects = new GameObject*[mCountOfObjects];
	for ( unsigned int i = 0; i < mCountOfObjects; i++ )
	{
		int modelId;
		int shaderId;
		int n_textureIds = 0;
		int n_cubetexIds = 0;
		int* textureIds = NULL;
		int* cubetexIds = NULL;

		fscanf(ptr_f, "ID %*d\n");
		fscanf(ptr_f, "MODEL %d\n", &modelId);
		fscanf(ptr_f, "TEXTURES %d\n", &n_textureIds);
		textureIds = new int[n_textureIds];
		for ( int j = 0; j < n_textureIds; j++ )
		{
			fscanf(ptr_f, "TEXTURE %d\n", &textureIds[j]);
		}

		fscanf(ptr_f, "CUBETEXTURES %d\n", &n_cubetexIds);
		cubetexIds = new int[n_cubetexIds];
		for ( int j = 0; j < n_cubetexIds; j++ )
		{
			fscanf(ptr_f, "CUBETEX %d\n", &cubetexIds[j]);
		}
		fscanf(ptr_f, "SHADER %d\n", &shaderId);
		mObjects[i] = new GameObject(modelId, shaderId, textureIds, n_textureIds, cubetexIds, n_cubetexIds);
		Vector3 num;
		fscanf(ptr_f, "POSITION %f, %f, %f\n", &num.x, &num.y, &num.z);
		mObjects[i]->mPostion = num;
		fscanf(ptr_f, "ROTATION %f, %f, %f\n", &num.x, &num.y, &num.z);
		mObjects[i]->mRotation = num;
		fscanf(ptr_f, "SCALE %f, %f, %f\n", &num.x, &num.y, &num.z);
		mObjects[i]->mScaling = num;

		int checkVisible;
		fscanf(ptr_f, "VISIBLE %d\n", &checkVisible);
		mObjects[i]->setVisible(checkVisible ? true:false);

		delete[] textureIds;
		delete[] cubetexIds;
	}
}

void SceneManager::initMenus(FILE* ptr_f)
{
	fscanf(ptr_f, "#Menus: %d\n", &mCountOfMenus);
	
	int modelId = 0;
	int shaderId = 0;
	fscanf(ptr_f, "MODEL %d\n", &modelId);
	fscanf(ptr_f, "SHADER %d\n", &shaderId);

	mMenus = new Control2D* [mCountOfMenus];
	int id;
	int textureId;
	int configId;
	for (int i = 0; i < mCountOfMenus; ++i)
	{
		fscanf(ptr_f, "ID %d\n", &id);
		fscanf(ptr_f, "TEXTURE %d\n", &textureId);
		fscanf(ptr_f, "CONFIG %d\n", &configId);
		mMenus[id] = new Control2D(modelId, shaderId, textureId, configId);
	}
}

void SceneManager::init(const char *smFile)
{
	FileManager *file = FileManager::getInstance();
	FILE *f = file->openFile(smFile, "r");
	if ( f == NULL )
	{
		printf("Can't open resource manager file\n");
		return;
	}
	else
	{
		initObjects(f);
		initMenus(f);
		initCamera(f);
	}
	fclose(f);
}

void SceneManager::render()
{
	for (unsigned int i = 0; i < mCountOfObjects; i++ )
	{
		if(mObjects[i]->isVisible())
		{
			mObjects[i]->render(mCamera);
		}
	}
	for(unsigned int i = 0; i < mCountOfMenus; i++)
	{
		if(mMenus[i]->getIsVisible())
		{
			mMenus[i]->render();
		}
	}
}

void SceneManager::update()
{
	mCamera->update();
	for ( unsigned int i = 0; i < mCountOfObjects; i++ )
	{
		mObjects[i]->update();
	}
	for(unsigned int i = 0; i < mCountOfMenus; i++)
	{
		if(mMenus[i]->getIsVisible())
		{
			mMenus[i]->update();
		}
	}
}

SceneManager::~SceneManager()
{
	FREE_2D_ARRAY(mObjects, mCountOfObjects);
	FREE_2D_ARRAY(mMenus, mCountOfMenus);
	SAFE_FREE(mCamera);
}