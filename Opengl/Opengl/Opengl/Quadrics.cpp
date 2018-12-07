#include "Quadrics.h"
#include "gl/GL.h"
#include "gl/GLU.h"

static GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
static GLfloat diffuseLight[] = { 1.0f, 0.8f, 0.6f, 1.0f };
static GLfloat specularLight[] = { 0.0f, 0.0f, 1.0f, 1.0f };
static GLfloat positionLight[] = { 6.0f, 0.0f, 6.0f, 1.0f };

static GLfloat ambientMaterial[] = { 0.3f, 0.2f, 0.3f, 1.0f };
static GLfloat diffuseMaterial[] = { 0.2f, 1.0f, 0.2f, 1.0f };
static GLfloat specularMaterial[] = { 0.0f, 0.0f, 1.0f, 1.0f };
static GLfloat shinessMaterial[] = { 5.0f };

bool Quadrics::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);
	glLineWidth(2.0f);

	//light
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//material
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, shinessMaterial);

	InitQuadric();
	return true;
}

bool Quadrics::DeInitialize()
{
	gluDeleteQuadric(m_pQuadric);
	return true;
}

void Quadrics::Update(DWORD dMillSeconds)
{
	m_Angle += (float)dMillSeconds / 15.0f;
}

void Quadrics::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//draw Sphere
	glPushMatrix();
		gluQuadricDrawStyle(m_pQuadric, GLU_FILL);
		glTranslatef(-1.0f, 1.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(0.5f, 0.2f, 0.2f);
		gluSphere(m_pQuadric, 0.5f, 100, 100);
	glPopMatrix();

	//draw cylinder
	glPushMatrix();
		gluQuadricDrawStyle(m_pQuadric, GLU_LINE);
		glTranslatef(1.0f, 1.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(0.2f, 0.5f, 0.2f);
		gluCylinder(m_pQuadric, 0.5f, 0.0f, 1.0f, 10, 10);
	glPopMatrix();

	//draw disk
	glPushMatrix();
		gluQuadricDrawStyle(m_pQuadric, GLU_POINT);
		glTranslatef(-1.0f, -1.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(0.2f, 0.2f, 0.5f);
		gluDisk(m_pQuadric, 0.0f, 0.5f, 100, 100);
	glPopMatrix();

	//draw patialDisk
	glPushMatrix();
		gluQuadricDrawStyle(m_pQuadric, GLU_LINE);
		glTranslatef(1.0f, -1.0f, -5.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(0.6f, 0.5f, 0.2f);
		gluPartialDisk(m_pQuadric, 0.0f, 0.5f, 10, 10, 0.0, 270.0);
	glPopMatrix();
}

Quadrics::Quadrics(const char* pClassName)
:GLApplication(pClassName)
{

}

void Quadrics::InitQuadric()
{
	m_pQuadric = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric, GLU_FILL);
	gluQuadricNormals(m_pQuadric, GLU_FLAT);
	gluQuadricOrientation(m_pQuadric, GLU_OUTSIDE);
}
