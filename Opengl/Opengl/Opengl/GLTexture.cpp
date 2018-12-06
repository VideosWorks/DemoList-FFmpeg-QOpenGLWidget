#include "GLTexture.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#define BITMAP_ID 0x4D42

GLTexture::GLTexture() :textureID(-1)
{

}

bool GLTexture::LoadTexture(LPSTR strFileName)
{
	if (!strFileName)
		return false;

	tImage* pImage = NULL;
	if (strstr(strFileName, ".tga"))
	{
		pImage = LoadTGA(strFileName);
	} 
	else if (strstr(strFileName, ".bmp"))
	{
		pImage = LoadBMP(strFileName);
	}
	else
	{
		return false;
	}

	if (!pImage)
		return false;

	glGenTextures(1, &textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int nTextureType = GL_RGB;
// 	if (pImage->channels == 4)
// 		nTextureType = GL_RGBA;

	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, pImage->sizeY, nTextureType, GL_UNSIGNED_BYTE, pImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	if (pImage)
	{
		if (pImage->data)
			free(pImage->data);
		free(pImage);
	}

	return true;
}

tImage* GLTexture::LoadBMP(const char* strFileName)
{
	return NULL;
}

tImage* GLTexture::LoadTGA(const char* strFileName)
{
	return NULL;
}

