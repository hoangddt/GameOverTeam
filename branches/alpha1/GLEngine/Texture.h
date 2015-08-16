#pragma once
#include "../Include/GLES2/gl2.h"
#include <stddef.h>

class Texture
{
private:
	void initCubetex(const char* img_data, int width, int height, int bpp) const;
	void initCubeFace(
		GLenum target, 
		GLenum format, 
		const char* img_data, 
		int position_x,
		int position_y,
		int frag_width, 
		int frag_height,
		int width,
		int bytes_per_pixel) const;
	char* getImageFrag(
		const char* p_src, 
		int frag_width, 
		int frag_height,
		int width,
		int padding_left,
		int padding_top,
		int bytes_per_pixel) const;
	GLenum mTextureMode;
	char* mPath;
public:
	GLuint mTextureId;
	GLenum mTiling;
	void init(const char* path);
	void init();
	static GLenum getTilingByString(const char* tilingStr);
	Texture(bool is2DTexture = true, const char* path = NULL);
	~Texture(void);
};