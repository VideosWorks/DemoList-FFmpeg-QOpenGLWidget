#ifndef _FOG_H
#define _FOG_H
#include "GLApplication.h"

class Fog : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Fog(const char* pClassName);
private:
	void DrawTriangls(float x, float y, float z);
};
#endif