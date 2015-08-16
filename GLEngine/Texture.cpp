#include "Texture.h"
#include "../Utilities/TGA.h"
#include "Lib.h"
#include <string.h>
#include "FileManager.h"
#include "LogHeaderAndroid.h"

void Texture::initCubetex(const char* img_data, int width, int height, int bpp) const
{
	int bytes_per_pixel = bpp / 8;
	int frag_width = width / 4;
	int frag_height = height / 3;
	GLenum format = bpp == 24 ? GL_RGB : GL_RGBA;
	// load top
	initCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, format, img_data, 1, 0, frag_width, frag_height, width, bytes_per_pixel);
	// load front
	initCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, format, img_data, 1, 1, frag_width, frag_height, width, bytes_per_pixel);
	// load right
	initCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_X, format, img_data, 2, 1, frag_width, frag_height, width, bytes_per_pixel);
	// load left
	initCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, format, img_data, 0, 1, frag_width, frag_height, width, bytes_per_pixel);
	// load bottom
	initCubeFace(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, format, img_data, 1, 2, frag_width, frag_height, width, bytes_per_pixel);
	// load back
	initCubeFace(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, format, img_data, 3, 1, frag_width, frag_height, width, bytes_per_pixel);
}
void Texture::initCubeFace(
						   GLenum target, 
						   GLenum format, 
						   const char* img_data, 
						   int position_x,
						   int position_y,
						   int frag_width, 
						   int frag_height,
						   int width,
						   int bytes_per_pixel) const
{
	int padding_top = position_y * frag_height;
	int padding_left = position_x * frag_width;
	char* buff = getImageFrag(img_data, frag_width, frag_height, width, padding_left, padding_top, bytes_per_pixel);
	glTexImage2D(target, 0, format, frag_width, frag_height, 0, format, GL_UNSIGNED_BYTE, buff);
	delete[] buff;
}

char* Texture::getImageFrag(
							const char* img_data, 
							int frag_width, 
							int frag_height,
							int width,
							int padding_left,
							int padding_top,
							int bytes_per_pixel) const
{
	char* buff = new char[frag_width * frag_height * bytes_per_pixel];
	char* p_des = buff;
	for(int i = frag_height - 1; i >= 0; i--)
	{
		if(bytes_per_pixel == 3)
		{
			const char* p_src = img_data + ((padding_top + i) * width + padding_left) * bytes_per_pixel;
			for(int j = frag_width - 1; j >= 0; j--)
			{
				*p_des ++ = *(p_src + 0);
				*p_des ++ = *(p_src + 1);
				*p_des ++ = *(p_src + 2);
				p_src += 3;
			}
		}
		else
		{
			for(int j = 0; j < frag_width; j++)
			{
				
			}
		}
	}
	return buff;
}

void Texture::init(const char* path)
{
	glGenTextures(1, &mTextureId);
	glBindTexture(mTextureMode, mTextureId);
	
	int width = 0;
	int height = 0;
	int bpp = 0;
	const char* imgData;
	GLenum format;

	if(path != NULL)
	{
		FileManager* fileManager = FileManager::getInstance();
		const char* filePath = fileManager->combineWithRoot(path);
		
		imgData = LoadTGA(filePath, width, height, bpp);
		delete[] filePath;
		format = bpp == 24 ? GL_RGB : GL_RGBA;
	}
	else
	{
		imgData = mBuffer;
		width = mImageWidth;
		height = mImageHeight;
		format = GL_RGBA;
		bpp = 32;
	}

	// load texture 2D
	if(mTextureMode == GL_TEXTURE_2D)
	{
		glTexImage2D(mTextureMode, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);
	}
	// load texture cube map
	else
	{
		initCubetex(imgData, width, height, bpp);
	}
	if(path != NULL)
	{
		delete[] imgData;
	}
	else
	{
		SAFE_FREE(mBuffer);
	}

	glTexParameteri(mTextureMode, GL_TEXTURE_WRAP_S, mTiling);
	glTexParameteri(mTextureMode, GL_TEXTURE_WRAP_T, mTiling);
	glTexParameteri(mTextureMode, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(mTextureMode, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(mTextureMode);
	glBindTexture(mTextureMode, 0);
}

void Texture::init()
{
	init(mPath);
}

Texture::Texture(bool is2DTexture, const char* path, GLenum tiling)
{	
	mTextureMode = is2DTexture ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;
	if(path != NULL)
	{
		this->mPath = new char[strlen(path) + 1];
		strcpy(this->mPath, path);
	}
	else
	{
		this->mPath = NULL;
	}
	this->mTiling = tiling;
	mBuffer = NULL;
}

Texture::Texture(const char* buffer, int imageWidth, int imageHeight)
{
	int len = imageWidth * imageHeight * 4;// support only 32bits image
	mBuffer = new char[len];
	for(int i = 0; i < len; i++)
	{
		mBuffer[i] = buffer[i];
	}
	mImageWidth = imageWidth;
	mImageHeight = imageHeight;
	mTextureMode = GL_TEXTURE_2D;
	mPath = NULL;
	mTiling = GL_CLAMP_TO_EDGE;
}

GLenum Texture::getTilingByString(const char* tilingStr)
{
	if (strstr("GL_REPEAT", tilingStr))
	{
		return GL_REPEAT;
	}
	if (strstr("GL_CLAMP_TO_EDGE", tilingStr))
	{
		return GL_CLAMP_TO_EDGE;
	}
	if (strstr("GL_MIRRORED_REPEAT", tilingStr))
	{
		return GL_MIRRORED_REPEAT;
	}
	if (strstr("GL_LINEAR", tilingStr))
	{
		return GL_LINEAR;
	}

	return GL_REPEAT;
}


Texture::~Texture(void)
{
	glBindTexture(mTextureMode, 0);
	glDeleteTextures(1, &mTextureId);
	FREE_1D_ARRAY(mPath);
	FREE_1D_ARRAY(mBuffer);
}
