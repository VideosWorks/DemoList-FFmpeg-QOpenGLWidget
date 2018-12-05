#ifndef _GLWINDOWS_H
#define _GLWINDOWS_H

#include <windows.h>

//windows窗口类

class GLWindow
{
public:
	GLWindow();
	//窗口创建
	bool Create(const char* pWindowsTitle, bool bFullScreen, const char* pClassName, HINSTANCE hInstance, LPVOID lpParam);
	//删除窗口
	void Destory();
	//改变窗口的显示设置
	bool ChangeScreenSetting();
	//响应窗口变化
	void ReshapeGL();
	//交换双缓存
	void SwapBuffers();

	//设置/修改窗口的属性
	void SetPosX(int nX);
	void SetPosX(unsigned short nX);
	void SetPosY(int nY);
	void SetPosY(unsigned short nY);
	int GetPosX();
	int GetPosY();

	//窗口的宽高
	void SetWidth(int nWidth);
	int GetWidth();
	void SetHeight(int nHeight);
	int GetHeight();

	HWND hWnd();
private:
	HWND m_hWnd; //窗口句柄
	HDC m_hDC; //设备描述符
	HGLRC m_hRC; //opengl渲染描述表

	int m_nWindowX, m_nWindowY;
	int m_nWindowWidth, m_nWindowHeight;
	int m_nScreenWidth, m_nScreenHeight;
	bool m_bIsFullScreen;
};

#endif