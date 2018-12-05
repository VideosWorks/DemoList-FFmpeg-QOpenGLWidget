#ifndef _EXAMPLE_H
#define _EXAMPLE_H

#include "GLApplication.h"

class Example : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Example(const char* pClassName);
private:
	friend class GLApplication;
	float m_Angle;
};

#endif