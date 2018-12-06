#ifndef _DISPLAYLIST_H
#define _DISPLAYLIST_H
#include "GLApplication.h"

class DisplayList : public GLApplication
{
public:
	virtual bool Initialize() override;
	virtual bool DeInitialize() override;
	virtual void Update(DWORD dMillSeconds) override;
	virtual void Draw() override;

	DisplayList(const char* pClassName);
private:
	void initDisplayList();

private:
	float m_Angle;
	unsigned int m_displayListIndex;
};

#endif