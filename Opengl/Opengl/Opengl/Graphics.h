#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#include "GLApplication.h"

class Graphics : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Graphics(const char* pClassName);
private:
	float m_Angle;
};

#endif