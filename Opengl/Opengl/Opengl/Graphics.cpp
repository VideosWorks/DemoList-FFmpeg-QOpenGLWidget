#include "Graphics.h"
#include "gl/GL.h"

static GLfloat vertexts_six_faces[] = {1.0f, 1.0f, -1.0f,
										-1.0f, 1.0f, -1.0f,
										-1.0f, 1.0f, 1.0f,
										1.0f, 1.0f, 1.0f,
										1.0f, -1.0f, 1.0f,
										-1.0f, -1.0f, 1.0f,
										-1.0f, -1.0f, -1.0f,
										1.0f, -1.0f, -1.0f};

static GLfloat colors_six_faces[] = { 1.0f, 0.0f, 0.0f,
									  1.0f, 0.0f, 0.0f,
									  0.0f, 1.0f, 0.0f,
									  0.0f, 1.0f, 0.0f,
									  0.0f, 0.0f, 1.0f,
									  0.0f, 0.0f, 1.0f,
									  1.0f, 1.0f, 1.0f,
									  1.0f, 1.0f, 1.0f};

static GLbyte indexs_six_faces[] = { 0, 1, 2, 3,    //top
									  4, 5, 6, 7,	    //bottom
 									  0, 3, 4, 7,    //right
 									  2, 5, 4, 3,    //front
 									  2, 1, 6, 5,    //left 
 									  0, 7, 6, 1};   //back


static GLfloat vertexts_four_faces[] = {0.0f, 0.0f, 0.0f,
									1.0f, 0.0f, 0.0f, 
									0.5f, 0.0f, -1.0f,
									0.5f, 1.0f, -0.5f};

static GLfloat colors_four_faces[] = { 1.0f, 0.2f, 0.2f,
									1.0f, 0.2f, 0.2f,
									0.2f, 1.0f, 0.2f,
									0.2f, 1.0f, 0.2f };

static GLbyte indexs_four_faces[] = {0, 2, 1, //bottom
									0, 1, 3,  //front
									0, 3, 2,  //left
									1, 2, 3}; //right

bool Graphics::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	return true;
}

bool Graphics::DeInitialize()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	return true;
}

void Graphics::Update(DWORD dMillSeconds)
{
	m_Angle += (float)dMillSeconds / 15.0f;
}

void Graphics::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//draw six faces

	glPushMatrix();
		glTranslatef(-1.5f, 0.0f, -7.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glVertexPointer(3, GL_FLOAT, 0, vertexts_six_faces);
		glColorPointer(3, GL_FLOAT, 0, colors_six_faces);
		glBegin(GL_QUADS);
			for (int i = 0; i < 6; ++i)
			{
				glArrayElement(indexs_six_faces[4 * i]);
				glArrayElement(indexs_six_faces[4 * i + 1]);
				glArrayElement(indexs_six_faces[4 * i + 2]);
				glArrayElement(indexs_six_faces[4 * i + 3]);
			}
 		glEnd();
	glPopMatrix();

	//draw four faces
	glPushMatrix();
		glTranslatef(1.5f, 0.0f, -7.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 0.0f);
		glVertexPointer(3, GL_FLOAT, 0, vertexts_four_faces);
		glColorPointer(3, GL_FLOAT, 0, colors_four_faces);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_BYTE, indexs_four_faces); // type不能是GL_BYTE否则无渲染效果
	glPopMatrix();
}

Graphics::Graphics(const char* pClassName)
:GLApplication(pClassName)
{

}

