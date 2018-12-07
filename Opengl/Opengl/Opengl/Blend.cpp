#include "Blend.h"
#include "GL/GL.h"
#include "GL/GLU.h"

bool Blend::Initialize()
{
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); //控制行为 处理反走样

	InitTexture();
	return true;
}

bool Blend::DeInitialize()
{
	m_Texture[0].FreeImage();
	m_Texture[1].FreeImage();
	glDisable(GL_TEXTURE_2D);
	return true;
}

void Blend::Update(DWORD dMillSeconds)
{
	m_Angle += (float)dMillSeconds / 15.0f;
}

void Blend::Draw()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//绘制不透明窗体
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 0.0f);
		DrawBox(m_Texture[0].ID, 0.5f);
	glPopMatrix();

	//绘制透明窗体
	glPushMatrix();
		glTranslatef(0.0f, 0.0f, -8.0f);
		glRotatef(m_Angle, 1.0f, 1.0f, 0.0f);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		DrawBox(m_Texture[1].ID, 1.0f);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
	glPopMatrix();
}

void Blend::InitTexture()
{
	glEnable(GL_TEXTURE_2D);

	if (!m_Texture[0].LoadBitmap("image/image.bmp"))
		return;

	if (!m_Texture[1].LoadBitmap("image/wall.bmp"))
		return;

	glGenTextures(1, &m_Texture[0].ID);
	glBindTexture(GL_TEXTURE_2D, m_Texture[0].ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture[0].imageWidth, m_Texture[0].imageHeight, GL_RGB, GL_UNSIGNED_BYTE, m_Texture[0].image);

	glGenTextures(1, &m_Texture[1].ID);
	glBindTexture(GL_TEXTURE_2D, m_Texture[1].ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture[1].imageWidth, m_Texture[1].imageHeight, GL_RGB, GL_UNSIGNED_BYTE, m_Texture[1].image);
}

void Blend::DrawBox(unsigned int nID, float r)
{
	glBindTexture(GL_TEXTURE_2D, nID);
	glBegin(GL_QUADS);
		// 前侧面
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, -r, r);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(r, -r, r);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(r, r, r);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, r, r);
		// 后侧面
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-r, -r, -r);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-r, r, -r);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(r, r, -r);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(r, -r, -r);
		// 顶面
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, r, -r);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, r, r);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(r, r, r);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(r, r, -r);
		// 底面
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-r, -r, -r);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(r, -r, -r);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(r, -r, r);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-r, -r, r);
		// 右侧面
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(r, -r, -r);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(r, r, -r);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(r, r, r);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(r, -r, r);
		// 左侧面
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-r, -r, -r);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-r, -r, r);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-r, r, r);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-r, r, -r);
	glEnd();
}

Blend::Blend(const char* pClassName)
:GLApplication(pClassName)
{

}

