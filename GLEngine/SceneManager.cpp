#include "SceneManager.h"
#include "FileManager.h"
#include "Texture.h"
#include "Lib.h"
#include "ControlButton.h"

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
	mButtons = NULL;
	mCountOfButtons = 0;
	mMapObject = NULL;;
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

void SceneManager::initButtons(FILE *ptr_f)
{
	mCountOfButtons = 0;
	fscanf(ptr_f, "#Buttons: %d\n", &mCountOfButtons);
	mButtons = new Button*[mCountOfButtons];
	for ( unsigned int i = 0; i < mCountOfButtons; i++ )
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
		mButtons[i] = new Button(modelId, shaderId, textureIds, n_textureIds, cubetexIds, n_cubetexIds);
		Vector3 num;
		fscanf(ptr_f, "POSITION %f, %f, %f\n", &num.x, &num.y, &num.z);
		mButtons[i]->mPostion = num;
		fscanf(ptr_f, "ROTATION %f, %f, %f\n", &num.x, &num.y, &num.z);
		mButtons[i]->mRotation = num;
		fscanf(ptr_f, "SCALE %f, %f, %f\n", &num.x, &num.y, &num.z);
		mButtons[i]->mScaling = num;

		int checkVisible;
		fscanf(ptr_f, "VISIBLE %d\n", &checkVisible);
		mButtons[i]->setVisible(checkVisible ? true:false);

		delete[] textureIds;
		delete[] cubetexIds;
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
		initButtons(f);
		initCamera(f);
	}
	fclose(f);
	mSystemEvent = new SystemEvent(mButtons, mCountOfButtons);
	mMapObject = NULL;
	ControlButton::getInstance()->init();
	ControlButton::getInstance()->setPosition(0.0, 0.2, 0.0);
	ControlButton::getInstance()->setScaling(0.3, 0.5, 0.5);
}

void SceneManager::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (unsigned int i = 0; i < mCountOfObjects; i++ )
	{
		if(mObjects[i]->isVisible())
		{
			mCamera->setObjectType(true); //3D
			mObjects[i]->render(mCamera);
		}
	}
	if(mMapObject != NULL)
	{
		mMapObject->render();
	}
	for (unsigned int i = 0; i < mCountOfButtons; i++ )
	{
		if(mButtons[i]->isVisible())
		{
			mCamera->setObjectType(false); //2D
			if ( mCamera->isUp() )
			{
				printf("Vo duoc UP\n");
				mButtons[i]->mRotation = CAMERA_ROTATION_FRONT;
				mButtons[i]->mPostion = CAMERA_POSITION_UP;
				mButtons[i]->mPostion.y -= 2.0f;
				mButtons[i]->render(mCamera);
			}
			if ( mCamera->isFront() )
			{
				printf("Vo duoc FRONT\n");
				mButtons[i]->mRotation = CAMERA_ROTATION_UP;
				mButtons[i]->mPostion = CAMERA_POSITION_FRONT;
				mButtons[i]->mPostion.z = 2.0f;
				mButtons[i]->render(mCamera);
			}
			//printf("Button %f, %f, %f\n", mButtons[i]->mRotation.x, mButtons[i]->mRotation.y, mButtons[i]->mRotation.z); 
			
		}
	}
	
	mCamera->setObjectType(false); //2D
	ControlButton::getInstance()->render(mCamera);
	
}

void SceneManager::update()
{
	for ( unsigned int i = 0; i < mCountOfObjects; i++ )
	{
		mObjects[i]->update();
	}
	for ( unsigned int i = 0; i < mCountOfButtons; i++ )
	{
		mButtons[i]->update();
	}
	mCamera->update();
	if(mMapObject != NULL)
	{
		mMapObject->update();
	}
	ControlButton::getInstance()->update();
	ControlButton::getInstance()->checkClick();
	mSystemEvent->loop();
}

void SceneManager::genMapObject(int** map, int row, int col, GameObject* orginObject)
{
	SAFE_FREE(mMapObject);
	mMapObject = new MapObject((const int **)map, row, col, orginObject);
	/*mMapObject->mPosition.z = -0.5f;
	mMapObject->updateObjectsPosition();*/
}

SceneManager::~SceneManager()
{
	FREE_2D_ARRAY(mObjects, mCountOfObjects);
	FREE_2D_ARRAY(mButtons, mCountOfButtons);
	SAFE_FREE(mMapObject);
	SAFE_FREE(mCamera);
	SAFE_FREE(mSystemEvent);
	ControlButton::destroyInstance();
}

Button* SceneManager::getControlButton()
{
	// return last button
	// last button is control button
	return mButtons[mCountOfButtons-1];
}