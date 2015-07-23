#include "ControlButton.h"
#include "SceneManager.h"
#include "Application.h"


ControlButton* ControlButton::mInstance = NULL;

ControlButton* ControlButton::getInstance()
{
	if (mInstance == NULL)
		mInstance = new ControlButton;
	return mInstance;
}
void ControlButton::destroyInstance()
{
	delete mInstance;
}

ControlButton::ControlButton()
{
	mButtons = NULL;
	mCountOfButtons = 0;
}

ControlButton::~ControlButton()
{
	this->destroy();
}

void ControlButton::init()
{
	// Create 3 button
	Button* originalButton = SceneManager::getInstance()->getControlButton();
	mCountOfButtons = 3;
	mButtons = new Button*[mCountOfButtons];

	for (int i = 0; i < mCountOfButtons; i++)
	{
		mButtons[i] = originalButton->clone();
		// remove the statement below if meet error
		mButtons[i]->init();
	}

	float angle = 15.0f;
	// mButtons[0]->mRotation.z = -angle;
	// mButtons[2]->mRotation.z = angle;

}
void ControlButton::update()
{
	for (int i = 0; i < mCountOfButtons; i++)
	{
		mButtons[i]->update();
	}
}
void ControlButton::render(Camera* camera)
{
	for (int i = 0; i < mCountOfButtons; i++)
	{
		mButtons[i]->render(camera);
	}
}
void ControlButton::destroy()
{
	for (int i = 0; i < mCountOfButtons; i++)
	{
		delete mButtons[i];
	}
	delete mButtons;
}

void ControlButton::setPosition(float x, float y, float z)
{
	// retrieve x, y, z
	// All will have same y, z
	// x will different for each elements
	for (int i = 0; i < mCountOfButtons; i++)
	{
		mButtons[i]->mPostion.y = y;
		mButtons[i]->mPostion.z = z;
	}
	
	int middle = mCountOfButtons / 2;
	mButtons[middle]->mPostion.x = x;

	float width = mButtons[middle]->getActualWidth();
	
	mButtons[middle-1]->mPostion.x = float ( x - width/2 - 0.2 * width );
	mButtons[middle+1]->mPostion.x = float ( x + width/2 + 0.2 * width );
}

void ControlButton::setScaling(float x, float y, float z)
{
	for (int i = 0; i < mCountOfButtons; i++)
	{
		mButtons[i]->mScaling.x = x;
		mButtons[i]->mScaling.y = y;
		mButtons[i]->mScaling.z = z;
	}
}

// return BUTTON_LEFT,	BUTTON_MIDDLE,	BUTTON_RIGHT, BUTTON_NONE
ButtonState ControlButton::checkClick()
{
	InputManager* input = InputManager::getInstance();
	if (input->hasTouch(TOUCH_DOWN))
	{
		TouchData touchData= input->getTouchData();
		printf("Touch: %d  %d\n", touchData.x, touchData.y);
		for (int i = 0; i < mCountOfButtons; i++)
		{
			if (mButtons[i]->containCoord(touchData.x, touchData.y))
			{
				switch (i)
				{
					case 0:
						printf("BUTTON_LEFT\n");
						return BUTTON_LEFT;
					case 1:
						printf("BUTTON_MIDDLE\n");
						return BUTTON_MIDDLE;
					case 2:
						printf("BUTTON_RIGHT\n");
						return BUTTON_RIGHT;
					default:
						printf("BUTTON_NONE\n");
						return BUTTON_NONE;
				}
			}
		}
	}
	else
	{
		printf("else BUTTON_NONE\n");
		return BUTTON_NONE;
	}
}