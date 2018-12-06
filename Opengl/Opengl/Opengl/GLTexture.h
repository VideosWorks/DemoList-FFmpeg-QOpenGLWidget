#ifndef _GLTEXTURE_H
#define _GLTEXTURE_H

#include <windows.h>
#include <fstream>

#define TGA_RGB 2
#define TGA_A     3
#define TGA_RLE 10

struct tImage
{
	int channels;
	int sizeX;
	int sizeY;
	unsigned char* data;
};

class GLTexture
{
public:
	GLTexture();
	//������Դ
	bool LoadTexture(LPSTR strFileName);
	//����ID
	UINT textureID;
private:
	tImage* LoadBMP(const char* strFileName);
	tImage* LoadTGA(const char* strFileName);
};
#endif