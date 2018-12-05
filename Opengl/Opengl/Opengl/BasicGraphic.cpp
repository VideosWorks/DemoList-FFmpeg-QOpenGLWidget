#include "BasicGraphic.h"
#include <math.h>
#include "gl/GL.h"

bool BasicGraphic::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glLineWidth(2.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	return true;
}

bool BasicGraphic::DeInitialize()
{
	return true;
}

void BasicGraphic::Update(DWORD dMillSeconds)
{
	m_Angle += float(dMillSeconds) / 15.0f;
}

void BasicGraphic::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//draw seperate line
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -7.0f);
		glColor3f(0.8f, 0.8f, 0.8f);
		glBegin(GL_LINES);
			glVertex3f(-10.0f, 0.0f, 0.0f);
			glVertex3f(10.0f, 0.0f, 0.0f);
			glVertex3f(0.0, 10.0f, 0.0f);
			glVertex3f(0.0, -10.0f, 0.0f);
		glEnd();
	glPopMatrix();

	//draw line
	glPushMatrix();
		glTranslatef(-1.0f, 1.0f, -7.0f);
		glRotatef(m_Angle, 1.0f, 0.0f, 0.0f);
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, 1.0f, 0.0f);
		glEnd();
	glPopMatrix();

	//draw circle
	glPushMatrix();
		glTranslatef(2.0f, 1.5f, -7.0f);
		glRotatef(m_Angle, 0.0f, 1.0f, 0.0f);
		glColor3f(0.0f, 0.5f, 0.0f);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0.0f, 0.0f, 0.0f);
			for (int i = 0; i <= 370; i += 10)
			{
				float p = (float)(i * 3.14 / 180);
				glVertex3f((float)sin(p), (float)cos(p), 0.0f);
			}
		glEnd();
	glPopMatrix();

	//draw triangle
	glPushMatrix();
		glTranslatef(-1.0f, -1.0f, -7.0f);
		glRotatef(m_Angle, 0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 0.0f, 0.5f);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, -1.0f, 0.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();

	//draw rect
	glPushMatrix();
		glTranslatef(1.0f, -1.0f, -7.0f);
		glColor3f(0.9f, 0.5f, 0.2f);
		glBegin(GL_QUADS);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, 0.0f, 0.0f);
			glVertex3f(1.0f, -1.0f, 0.0f);
			glVertex3f(0.0f, -1.0f, 0.0f);
		glEnd();
	glPopMatrix();
}

BasicGraphic::BasicGraphic(const char* pClassName)
:GLApplication(pClassName)
{

}

