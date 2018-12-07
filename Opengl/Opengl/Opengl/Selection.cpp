#include "Selection.h"
#include "gl/GL.h"
#include "gl/GLU.h"

#define CUBE1 1
#define CUBE2 2

int Rotate = 0;

bool Selection::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	return true;
}

bool Selection::DeInitialize()
{
	return true;
}

void Selection::Update(DWORD dMillSeconds)
{
	if (Rotate == CUBE1)
	{
		m_Angle += 2.0f;
	}
}

void Selection::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glInitNames();
	glPushName(0);

	glLoadName(CUBE1);
	glPushMatrix();
		glTranslatef(-1.0f, 0.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();


	glLoadName(CUBE2);
	glPushMatrix();
		glTranslatef(1.0f, 0.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();
}

Selection::Selection(const char* pClassName)
:GLApplication(pClassName), m_Angle(0.0f)
{

}

void Selection::ProcessObjects(int xPos, int yPos)
{
	//select buffer
	GLuint select[98];
	GLint viewport[4];
	GLint clickTest;
	int objectSelected;

	glSelectBuffer(98, select);
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glRenderMode(GL_SELECT);
		glLoadIdentity();

		gluPickMatrix(xPos, viewport[3] - yPos, 2, 2, viewport);
		gluPerspective(45.0f, 640 / 480, 1.0, 100.0);

		Draw();

		//check
		clickTest = glRenderMode(GL_RENDER);
		if (clickTest == 1)
		{
			objectSelected = select[3];
			switch (objectSelected)
			{
			case CUBE1:
				Rotate = 1;
				break;

			case CUBE2:
				Rotate = 2;
				break;

			default:
				break;
			}
		}

		glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

