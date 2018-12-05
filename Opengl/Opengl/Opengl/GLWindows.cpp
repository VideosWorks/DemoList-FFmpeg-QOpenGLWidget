#include "GLWindows.h"
#include "gl/GL.h"
#include "gl/GLU.h"

GLWindow::GLWindow()
{
	m_nWindowX = 400;  //default pos
	m_nWindowY = 200; //default pos
	m_nWindowWidth = 800; // default size
	m_nWindowHeight =  600; // default size
	m_nScreenWidth = 0;
	m_nScreenHeight = 0;
	m_bIsFullScreen = false;

	m_hWnd = 0;
	m_hDC = 0;
	m_hRC = 0;
}

bool GLWindow::Create(const char* pWindowsTitle, bool bFullScreen, const char* pClassName, HINSTANCE hInstance, LPVOID lpParam)
{
	m_bIsFullScreen = bFullScreen;

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1, 
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_STEREO,
		PFD_TYPE_RGBA,
		32, //颜色位深
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	DWORD windowExtendStyle = WS_EX_APPWINDOW;

	if (m_bIsFullScreen == true)
	{
		if (ChangeScreenSetting() == false)
		{
			MessageBox(HWND_DESKTOP, TEXT("模式转换失败\n在窗口模式下运行"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
			m_bIsFullScreen = false;
		} 
		else
		{
			ShowCursor(false);
			windowStyle = WS_POPUP;
			windowExtendStyle |= WS_EX_TOPMOST;
		}
	}

	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};

	if (m_bIsFullScreen == false)
	{
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendStyle);
		if (windowRect.left < 0)
		{
			windowRect.right -= windowRect.left;
			windowRect.left = 0;
		}

		if (windowRect.top < 0)
		{
			windowRect.bottom -= windowRect.top;
			windowRect.top = 0;
		}
	}

	//创建窗口
	m_hWnd = CreateWindowEx(windowExtendStyle, 
		pClassName,
		pWindowsTitle,
		windowStyle,
		windowRect.left,
		windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		HWND_DESKTOP,
		0,
		hInstance,
		lpParam);

	DWORD last = GetLastError();
	while (m_hWnd != 0)
	{
		//返回窗口的设备描述表
		m_hDC = GetDC(m_hWnd); 
		if (m_hDC == 0)
		{
			break;
		}

		GLuint pixelFormat = ChoosePixelFormat(m_hDC, &pfd);
		if (pixelFormat == 0)
		{
			break;
		}

		//设置像素格式
		if (SetPixelFormat(m_hDC, pixelFormat, &pfd) == false)
		{
			break;
		}

		//创建opengl的渲染描述表
		m_hRC = wglCreateContext(m_hDC);
		if (m_hRC == 0)
		{
			break;
		}

		//设置当前窗口
		if (wglMakeCurrent(m_hDC, m_hRC) == false)
		{
			break;
		}

		ShowWindow(m_hWnd, SW_NORMAL);
		ReshapeGL();
		return true;
	}

	Destory();
	return false;
}

bool GLWindow::ChangeScreenSetting()
{
	DEVMODE dmScreenSetting;
	ZeroMemory(&dmScreenSetting, sizeof(dmScreenSetting));
	dmScreenSetting.dmSize = sizeof(DEVMODE);
	dmScreenSetting.dmPelsWidth = GetWidth();
	dmScreenSetting.dmPelsHeight = GetHeight();
	dmScreenSetting.dmBitsPerPel = 32; //位深度
	dmScreenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&dmScreenSetting, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;
	}

	return true;
}

void GLWindow::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 100.0f); 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLWindow::Destory()
{
	if (m_hWnd != 0)
	{
		if (m_hDC != 0)
		{
			wglMakeCurrent(m_hDC, 0);
			if (m_hRC != 0)
			{
				wglDeleteContext(m_hRC); 
				m_hRC = 0; 
			}
			ReleaseDC(m_hWnd, m_hDC);
			m_hDC = 0;
		}
		//DestoryWindow(m_hWnd);
		m_hWnd = 0;
	}

	if (m_bIsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
}

void GLWindow::SwapBuffers()
{
	::SwapBuffers(m_hDC);
}

void GLWindow::SetPosX(int nX)
{
	m_nWindowX = nX;
}

void GLWindow::SetPosX(unsigned short nX)
{
	SetPosX((signed short)nX);
}

void GLWindow::SetPosY(int nY)
{
	m_nWindowY = nY;
}

void GLWindow::SetPosY(unsigned short nY)
{
	SetPosY((signed short)nY);
}

int GLWindow::GetPosX()
{
	return m_nWindowX;
}

int GLWindow::GetPosY()
{
	return m_nWindowY;
}

void GLWindow::SetWidth(int nWidth)
{
	m_nWindowWidth = nWidth;
}

int GLWindow::GetWidth()
{
	return m_nWindowWidth;
}

void GLWindow::SetHeight(int nHeight)
{
	m_nWindowHeight = nHeight;
}

int GLWindow::GetHeight()
{
	return m_nWindowHeight;
}

HWND GLWindow::hWnd()
{
	return m_hWnd;
}

