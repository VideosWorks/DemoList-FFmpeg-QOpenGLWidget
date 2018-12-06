#ifndef _H_LIGHT_H
#define _H_LIGHT_H
#include "GLApplication.h"

class Light : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Light(const char* pClassName);
private:
	float m_Angle; 
	float m_red;
};
#endif