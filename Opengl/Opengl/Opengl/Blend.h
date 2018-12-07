#ifndef _BLEND_H
#define _BLEND_H

#include "GLApplication.h"
#include "CBMPLoader.h"

class Blend : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Blend(const char* pClassName);
private:
	void InitTexture();
	void DrawBox(unsigned int nID, float r);
private:
	CBMPLoader m_Texture[2];
	float m_Angle;
};
#endif