#ifndef _GLWINDOWS_H
#define _GLWINDOWS_H

#include <windows.h>

//windows������

class GLWindow
{
public:
	GLWindow();
	//���ڴ���
	bool Create(const char* pWindowsTitle, bool bFullScreen, const char* pClassName, HINSTANCE hInstance, LPVOID lpParam);
	//ɾ������
	void Destory();
	//�ı䴰�ڵ���ʾ����
	bool ChangeScreenSetting();
	//��Ӧ���ڱ仯
	void ReshapeGL();
	//����˫����
	void SwapBuffers();

	//����/�޸Ĵ��ڵ�����
	void SetPosX(int nX);
	void SetPosX(unsigned short nX);
	void SetPosY(int nY);
	void SetPosY(unsigned short nY);
	int GetPosX();
	int GetPosY();

	//���ڵĿ��
	void SetWidth(int nWidth);
	int GetWidth();
	void SetHeight(int nHeight);
	int GetHeight();

	HWND hWnd();
private:
	HWND m_hWnd; //���ھ��
	HDC m_hDC; //�豸������
	HGLRC m_hRC; //opengl��Ⱦ������

	int m_nWindowX, m_nWindowY;
	int m_nWindowWidth, m_nWindowHeight;
	int m_nScreenWidth, m_nScreenHeight;
	bool m_bIsFullScreen;
};

#endif