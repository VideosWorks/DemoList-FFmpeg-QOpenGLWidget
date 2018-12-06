#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "GLApplication.h"
#include "CBMPLoader.h"

class Texture : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Texture(const char* pClassName);
private:
	void initTexture();
private:
	CBMPLoader m_Texture;
	float m_Angle; 
};
#endif