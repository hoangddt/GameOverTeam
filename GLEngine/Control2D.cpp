#include "Control2D.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "Camera2D.h"
#include <cmath>

void Control2D::initialize()
{
	mPosition = NULL;
	mNumOfElements = 0;
	mPath = NULL;
	mLastButtonIdDown = -1;
	mEnable = true;
	for(int i = 0; i < MENU_MAX_BUTTONS_SUPPORT; i++)
	{
		mButtonsDown[i] = false;
	}
}

void Control2D::setEnable(bool enable)
{
	mEnable = enable;
}

Control2D::Control2D()
{
	initialize();
}

Control2D::Control2D(int modelId, int shaderId, int textureId, int pathId)
{
	initialize();
	ResourceManager* rm = ResourceManager::getInstance();
	Model* model = rm->getModelById(modelId);
	Shader* shader = rm->getShaderById(shaderId);
	Texture* texture = rm->getTextureById(textureId);
	mPath = rm->getConfigById(pathId);
	mObject = new Game2DObject(model, shader, texture);
	mObject->mVisibility = VISIBILITY;
	mObject->setFullScreen();
	mObject->init();
	mPosition = NULL;
	mNumOfElements = 0;
	init();
}

void Control2D::init(const char *path)
{
	FileManager *file = FileManager::getInstance();
	FILE *f = file->openFile(path, "r");
	if ( f == NULL )
	{
		printf("Can't open file : %s\n", path);
		return;
	}
	else
	{
		mNumOfElements = 0;
		fscanf(f, "#ELEMENTS: %d\n", &mNumOfElements);
		mPosition = new Rect[mNumOfElements];
		Rect temp;
		for ( int i = 0; i < mNumOfElements; i++ )
		{
			fscanf(f ,"%*d. %f %f %f %f\n", &temp.left, &temp.top, &temp.right, &temp.bottom);
			mPosition[i] = temp;
		}
	}
	fclose(f);
}

void Control2D::init()
{
	init(mPath);
}

void Control2D::setVisible(Visibility visibility)
{
	mObject->mVisibility = visibility;
}

void Control2D::render()
{
	mObject->render();
}

bool Control2D::getIsVisible() const
{
	if ( mObject->mVisibility == VISIBILITY )
		return true;
	else
		return false;
}

void Control2D::setTexture(Texture* texture)
{
	mObject->mTexture = texture;
}

bool Control2D::isButtonPress(int id)
{
	if(!mEnable)
	{
		return false;
	}
	InputManager* inputManager = InputManager::getInstance();
	if(inputManager->hasTouch(TOUCH_DOWN))
	{
		TouchData data = InputManager::getInstance()->getTouchData();
		bool ret = Camera2D::getInstance()->isOnRect(data.x, data.y, mPosition[id]);
		mButtonsDown[id] = ret;
	}
	if(mButtonsDown[id] && inputManager->hasTouch(TOUCH_UP))
	{
		mButtonsDown[id] = false;
		TouchData data = InputManager::getInstance()->getTouchData();
		bool ret = Camera2D::getInstance()->isOnRect(data.x, data.y, mPosition[id]);
		return ret;
	}
	return false;
}

void Control2D::update()
{
	mObject->update();
}

Control2D::~Control2D()
{
	SAFE_FREE(mObject);
	SAFE_FREE(mPosition);
}