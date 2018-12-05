#include "Example.h"
#include "gl/GL.h"
#include "gl/GLU.h"

bool Example::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	ResizeDraw(true);

	return true;
}

bool Example::DeInitialize()
{
	return true;
}

void Example::Update(DWORD dMillSeconds)
{
	if (m_keys.IsPressed(VK_ESCAPE) == true)
	{
		TerminateApplication();
	}

	if (m_keys.IsPressed(VK_F1) == true)
	{
		ToggleFullScreen();
	}

	m_Angle += float(dMillSeconds) / 15.0f;
}

void Example::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0f, 0.0f, -7.0f);
	glRotatef(m_Angle, 0.0f, 1.0f, 0.0f);
	glColor3f(0.3f, 1.0f, 0.3f);
	
	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();
}

Example::Example(const char* pClassName)
:GLApplication(pClassName), m_Angle(0.0)
{
	
}

