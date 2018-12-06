#include "DisplayList.h"
#include "gl/GL.h"

bool DisplayList::Initialize()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	initDisplayList();
	return true;
}

bool DisplayList::DeInitialize()
{
	glDeleteLists(m_displayListIndex, 0);
	return true;
}

void DisplayList::Update(DWORD dMillSeconds)
{
	m_Angle += (float)dMillSeconds / 15.0f;
}

void DisplayList::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int x = 0; x < 3; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			glLoadIdentity();
			glTranslatef(-2.0f + 2 * x, -2.0f + 2 * y, -10.0f);
			glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
			glCallList(m_displayListIndex);
		}
	}
}

void DisplayList::initDisplayList()
{
	m_displayListIndex = glGenLists(1);
	if (m_displayListIndex != 0)
	{
		glNewList(m_displayListIndex, GL_COMPILE); //need match
			glBegin(GL_QUADS);
				glColor3f(1.0f, 0.0f, 0.0f);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glColor3f(0.0f, 1.0f, 0.0f);
				glVertex3f(0.0f, 1.0f, 0.0f);
				glColor3f(0.0f, 0.0f, 1.0f);
				glVertex3f(-1.0f, 1.0f, 0.0f);
				glColor3f(1.0f, 1.0f, 1.0f);
				glVertex3f(-1.0f, 0.0f, 0.0f);
			glEnd();
	    glEndList(); //need match
	}
}

DisplayList::DisplayList(const char* pClassName)
:GLApplication(pClassName)
{

}

