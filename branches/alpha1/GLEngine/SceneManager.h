#pragma once
#include "Lib.h"
#include "Camera.h"
#include "GameObject.h"
#include "MapObject.h"
#include "Button.h"
#include "InputManager.h"
#include "SystemEvent.h"

class SceneManager
{
private:
	static SceneManager* mInstance;
	SystemEvent* mSystemEvent;
	void initCamera(FILE* ptr_f);
	void initObjects(FILE* ptr_f);
	SceneManager();
	void initButtons(FILE* ptr_f);
	SceneManager(const SceneManager &){} 
	SceneManager& operator =(const SceneManager &){} 
	~SceneManager(void);
public:
	Camera* mCamera;
	GameObject** mObjects;
	uint mCountOfObjects;
	Button** mButtons;
	uint mCountOfButtons;

	static SceneManager* getInstance();
	static void destroyInstance();
	void init(const char* smFile);
	void genMapObject(int** map, int row, int col, GameObject* orginObject);
	void render();
	void update();
	Button* getControlButton();
};