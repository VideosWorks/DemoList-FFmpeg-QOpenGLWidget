#include "Fog.h"
#include "gl/GL.h"

static GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
static GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static GLfloat positionLight[] = {- 2.0f, 0.0f, 2.0f, 1.0f };

static GLfloat ambientMaterial[] = {0.0f, 0.2f, 0.8f, 1.0f };
static GLfloat diffuseMaterial[] = { 0.0f, 0.8f, 0.1f, 1.0f };
static GLfloat specularMaterial[] = { 1.0f, 0.0f, 0.0f, 1.0f };
static GLfloat shinessMaterial[] = {10.0f};

GLuint fogMode[] = {GL_LINEAR, GL_EXP, GL_EXP2};
GLuint fogFilter = 0;
GLfloat fogColor[4] = {0.0f, 0.5f, 0.8f, 1.0f};

bool Fog::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

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

	//fog
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, fogMode[fogFilter]);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 10.0f);
	glFogf(GL_FOG_DENSITY, 0.3f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);

	return true;
}

bool Fog::DeInitialize()
{
	return true;
}

void Fog::Update(DWORD dMillSeconds)
{
	//nothing
}

void Fog::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	for (int dis = 0; dis < 10; ++dis)
	{
		DrawTriangls(-3 + float(dis) * 0.8, 0.0f, -5.0f - float(dis) * 2);
	}
}

Fog::Fog(const char* pClassName)
:GLApplication(pClassName)
{
	
}

void Fog::DrawTriangls(float x, float y, float z)
{
	glPushMatrix();
		glScalef(0.5f, 0.5f, 0.5f);
		glTranslatef(x, y, z);
		glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(-0.5f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 0.0f, 0.0f);
		glEnd();
	glPopMatrix();
}