#pragma once
#include "StageManager.h"
#include "Game2DObject.h"

#define DESIGN_FONT_HEIGHT	128.0f
#define DESIGN_FONT_MARGIN	1.0f

class PlayingStageDrawer
{
private:
	static PlayingStageDrawer* mInstance;
	Game2DObject* mLevel;
	const char* mFontBuffer;
	int mWidth;
	int mHeight;
	bool mInitialized;

	PlayingStageDrawer();
	PlayingStageDrawer(const PlayingStageDrawer &){} 
	PlayingStageDrawer& operator =(const PlayingStageDrawer &){}
	~PlayingStageDrawer(void);
	const char* getFragImage(
		const char* img_data, 
		int frag_width, 
		int frag_height,
		int width,
		int position_x,
		int position_y) const;
	Texture* getFragTexture(int pos_x, int pos_y) const;
public:
	static PlayingStageDrawer* getInstance();
	static void destroyInstance();
	void setLevel(int level);
	void init(Model* model, Shader* shader, const char* fontFile);
	void render();
	void update();
	void destroy();
};
