#ifndef _SELECTION_H
#define _SELECTION_H
#include "GLApplication.h"

class Selection : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	Selection(const char* pClassName);
private:
	void ProcessObjects(int xPos, int yPos);
private:
	float m_Angle;
};
#endif