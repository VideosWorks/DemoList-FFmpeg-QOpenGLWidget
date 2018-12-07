#ifndef _QUADRICS_H
#define _QUADRICS_H
#include "GLApplication.h"
#include "gl/GLU.h"

class Quadrics : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Quadrics(const char* pClassName);
private:
	void InitQuadric();
private:
	float m_Angle; 
	GLUquadric* m_pQuadric;
};
#endif