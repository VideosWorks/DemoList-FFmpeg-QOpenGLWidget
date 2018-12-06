#include "Light.h"
#include "gl/GL.h"

//light
static GLfloat nDefaultDiffuseLight[] = {0.8f, 0.8f, 0.8f, 1.0f};
static GLfloat nDefaultSpecularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat nDefaultPositionLight[] = {-1.0f, 0.0f, 0.0f, 1.0f};

static GLfloat nCurrentDiffuseLight[] = {1.0f, 0.0f, 0.0f, 1.0f};
static GLfloat nCurrentSpecularLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
static GLfloat nCurrentPostionLight[] = { -1.0f, 0.0f, 0.0f, 1.0f };

//metiral
static GLfloat nDefaultAmbientMateral[] = {0.7f, 0.7f, 0.7f, 1.0f};
static GLfloat nDefaultDiffuseMaterial[] = {0.2f, 0.5f, 0.8f, 1.0f};
static GLfloat nDefaultSpecularMaterial[] = {1.0f, 1.0f, 1.0f, 1.0};
static GLfloat nDefaultShininessMaterial[] = {100.0f};

static GLfloat vertexts_six_faces[] = { 1.0f, 1.0f, -1.0f,
-1.0f, 1.0f, -1.0f,
-1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,
1.0f, -1.0f, 1.0f,
-1.0f, -1.0f, 1.0f,
-1.0f, -1.0f, -1.0f,
1.0f, -1.0f, -1.0f };

static GLbyte indexs_six_faces[] = { 0, 1, 2, 3,    //top
4, 5, 6, 7,	    //bottom
0, 3, 4, 7,    //right
2, 5, 4, 3,    //front
2, 1, 6, 5,    //left 
0, 7, 6, 1 };   //back

bool Light::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//顶点数组
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexts_six_faces);

	//0号光源
	glLightfv(GL_LIGHT0, GL_DIFFUSE, nDefaultDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, nDefaultSpecularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, nDefaultPositionLight);

	//1号光源
	glLightfv(GL_LIGHT1, GL_DIFFUSE, nCurrentDiffuseLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, nCurrentSpecularLight);
	glLightfv(GL_LIGHT1, GL_POSITION, nCurrentPostionLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//材质
	glMaterialfv(GL_FRONT, GL_AMBIENT, nDefaultAmbientMateral);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, nDefaultDiffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, nDefaultSpecularMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, nDefaultShininessMaterial);
	return true;
}

bool Light::DeInitialize()
{
	glDisable(GL_LIGHTING);
	return true;
}

void Light::Update(DWORD dMillSeconds)
{
	if (m_keys.IsPressed(VK_F1) == false)
	{
		glEnable(GL_LIGHT1);
		m_red += (float)dMillSeconds / 20.0f;
	} 
	else
	{
		glDisable(GL_LIGHT1);
	}
	m_Angle += (float)dMillSeconds / 10.0f;
}

void Light::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//绘制光源
	glPushMatrix();
		glTranslatef(-1.0f, 0.0f, -5.0f);
		glRotatef(m_red, 1.0f, 1.0f, 0.0f);
		glLightfv(GL_LIGHT1, GL_POSITION, nCurrentPostionLight);
		glColor3f(1.0f, 0.0f, 0.0f);

		glPushAttrib(GL_LIGHTING_BIT);
			glDisable(GL_LIGHTING);
			glBegin(GL_TRIANGLES);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glVertex3f(-0.25f, 0.5f, 0.0f);
				glVertex3f(-0.5f, 0.0f, 0.0f);
			glEnd();
			glEnable(GL_LIGHTING);
		glPopAttrib();

	glPopMatrix();

	//绘制立方体
	glPushMatrix();
		glTranslatef(2.0f, 0.0f, -15.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 0.0f);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexs_six_faces);
	glPopMatrix();
}

Light::Light(const char* pClassName)
:GLApplication(pClassName)
{

}

