#include "PlayingStageDrawer.h"
#include "Lib.h"
#include "LogHeaderAndroid.h"
#include "..\Utilities\TGA.h"
#include <GLES2\gl2.h>

PlayingStageDrawer* PlayingStageDrawer::mInstance = NULL;

PlayingStageDrawer* PlayingStageDrawer::getInstance()
{
	if(mInstance == NULL)
	{
		mInstance = new PlayingStageDrawer();
	}
	return mInstance;
}

void PlayingStageDrawer::destroyInstance()
{
	SAFE_FREE(mInstance);
}

const char* PlayingStageDrawer::getFragImage(
	const char* img_data, 
	int frag_width, 
	int frag_height,
	int width,
	int position_x,
	int position_y) const
{
	int padding_top = position_y * frag_height;
	int padding_left = position_x * frag_width;
	char* buff = new char[frag_width * frag_height * 4];
	char* p_des = buff;
	for(int i = 0; i < frag_height; i++)
	{
		const char* p_src = img_data + ((padding_top + i) * width + padding_left) * 4;
		for(int j = 0; j < frag_width; j++)
		{
			*p_des ++ = *(p_src + 0);
			*p_des ++ = *(p_src + 1);
			*p_des ++ = *(p_src + 2);
			*p_des ++ = *(p_src + 3);
			p_src += 4;
		}
	}
	return buff;
}

Texture* PlayingStageDrawer::getFragTexture(int pos_x, 
											int pos_y) const
{
	if(mFontBuffer == NULL)
	{
		printf("Must init font buffer before using it!\n");
		return NULL;
	}
	const char* frag = getFragImage(mFontBuffer, 
									mWidth, 
									mHeight / 8, 
									mWidth, 
									pos_x,
									pos_y);
	Texture* texture = new Texture(frag, mWidth, mHeight / 8);
	texture->mTiling = GL_CLAMP_TO_EDGE;
	texture->init();
	delete[] frag;
	return texture;
}

void PlayingStageDrawer::setLevel(int level)
{
	delete mLevel->mTexture;
	mLevel->mTexture = getFragTexture(0, 8 - level);
}

void PlayingStageDrawer::render()
{
	if(mInitialized)
	{
		mLevel->render();
	}
}

void PlayingStageDrawer::update()
{
	if(mInitialized)
	{
		mLevel->update();
	}
}

void PlayingStageDrawer::init(Model* model, 
							  Shader* shader, 
							  const char* fontFile)
{
	int bpp;
	const char* fileName = FileManager::getInstance()->combineWithRoot(fontFile);
	mFontBuffer = LoadTGA(fileName, mWidth, mHeight, bpp);
	delete[] fileName;
	if(bpp != 32)
	{
		printf("Only support 32 bits font image!\n");
		return;
	}
	
	mLevel = new Game2DObject(model, shader, getFragTexture(0, 0));
	mLevel->setSize(DESIGN_SCREEN_WIDTH, DESIGN_FONT_HEIGHT);
	mLevel->setPosition(
		(DESIGN_SCREEN_WIDTH - DESIGN_SCREEN_WIDTH) / 2.0f + DESIGN_FONT_MARGIN,
		-(DESIGN_FONT_HEIGHT - DESIGN_SCREEN_HEIGHT) / 2.0f - DESIGN_FONT_MARGIN);
	mLevel->init();

	mInitialized = true;
}

void PlayingStageDrawer::destroy()
{
	delete mLevel->mTexture;
	SAFE_FREE(mLevel);
	SAFE_FREE(mFontBuffer);
}

PlayingStageDrawer::PlayingStageDrawer()
{
	mLevel = NULL;
	mFontBuffer = NULL;
	mInitialized = false;
}

PlayingStageDrawer::~PlayingStageDrawer()
{
	destroy();
}