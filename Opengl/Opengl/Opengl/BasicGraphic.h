#ifndef _BASICGRAPHIC_H
#define _BASICGRAPHIC_H

#include "GLApplication.h"

class BasicGraphic : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	BasicGraphic(const char* pClassName);
private:
	float m_Angle;
};

#endif