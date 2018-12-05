#include "GLApplication.h"

#include "Example.h"
GLApplication* GLApplication::Create(const char* pClassName)
{
	//工厂模式创建Application
	Example* pExample = new Example("Example");
	return pExample;
}

GLApplication::GLApplication(const char* pClassName)
{
	m_pClassName = pClassName;
	m_IsProgramLooping = true;
	m_CreateFullScreen = true;
	m_IsVisible = false;
	m_ResizeDraw = false;
	m_LastTickCount = 0;
}

void GLApplication::ToggleFullScreen()
{
	PostMessage(m_Window.hWnd(), WM_TOGGLEFULLSCREEN, 0, 0);
}

void GLApplication::TerminateApplication()
{
	PostMessage(m_Window.hWnd(), WM_QUIT, 0, 0);
	m_IsProgramLooping = false;
}

LRESULT GLApplication::Message(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
		
	case WM_CLOSE:
		TerminateApplication();
		return 0;
		break;

	case WM_EXITMENULOOP:
	case WM_EXITSIZEMOVE:
		m_LastTickCount = GetTickCount();
		return 0;
		break;

	case WM_MOVE:
		m_Window.SetPosX(LOWORD(lParam));
		m_Window.SetPosY(HIWORD(lParam));
		return 0;
		break;

	case WM_PAINT:
		if (m_ResizeDraw == true)
		{
			m_Window.ReshapeGL();
			Draw();
			m_Window.SwapBuffers();
		}
		break;

	case WM_SIZING: //窗口正在改变大小
		{
			RECT* rect = (RECT*)lParam;
			m_Window.SetWidth(rect->right - rect->left);
			m_Window.SetHeight(rect->bottom - rect->top);
			return true;
			break;
		}

	case WM_SIZE: //窗口改变大小后
		switch (wParam)
		{
		case SIZE_MINIMIZED:
			m_IsVisible = false;
			return 0;
			break;

		case SIZE_MAXIMIZED:
		case SIZE_RESTORED:
			m_IsVisible = true;
			m_Window.SetWidth(LOWORD(lParam));
			m_Window.SetHeight(HIWORD(lParam));
			m_Window.ReshapeGL();
			m_LastTickCount = GetTickCount();
			return 0;
			break;
		}
		break;

	case WM_KEYDOWN:
		m_keys.SetPressed(wParam);
		return 0;
		break;
	
	case WM_KEYUP:
		m_keys.SetReleased(wParam);
		return 0;
		break;

	case WM_TOGGLEFULLSCREEN: //切换全屏和窗口模式
		m_CreateFullScreen = !m_CreateFullScreen;
		PostMessage(hwnd, WM_QUIT, 0, 0);
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int GLApplication::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX); //窗口结构大小
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)(WindowProc); //消息处理
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.lpszClassName = m_pClassName; //设置类名

	if (RegisterClassEx(&windowClass) == 0)
	{
		MessageBox(HWND_DESKTOP, TEXT("error"), TEXT("TEXT"), MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//do not neet fullscreen
// 	if (MessageBox(HWND_DESKTOP, TEXT("Fullscreen?"), TEXT("Hint"), MB_YESNO | MB_ICONQUESTION) == IDNO)
// 	{
// 		m_CreateFullScreen = false;
// 	}

	m_CreateFullScreen = false;
	while (m_IsProgramLooping)
	{
		if (m_Window.Create("OOGLFrame", m_CreateFullScreen, m_pClassName, hInstance, this)  == true)
		{
			if (Initialize() == false)
			{
				TerminateApplication();
			} 
			else
			{
				MSG msg;
				bool isMessagePumpActive = true;
				m_LastTickCount = GetTickCount();
				m_keys.Clear();
				while (isMessagePumpActive == true)
				{
					if (PeekMessage(&msg, m_Window.hWnd(), 0, 0, PM_REMOVE) != 0)
					{
						if (msg.message != WM_QUIT)
						{
							DispatchMessage(&msg);
						}
						else
						{
							isMessagePumpActive = false;
						}
					}
					else
					{
						if (m_IsVisible == false)
						{
							WaitMessage();
						} 
						else
						{
							DWORD tickCount = GetTickCount();
							Update(tickCount - m_LastTickCount);
							m_LastTickCount = tickCount;
							Draw();
							m_Window.SwapBuffers();
						}
					}
				}
				DeInitialize();
				m_Window.Destory();
			}
		} 
		else
		{
			MessageBox(HWND_DESKTOP, TEXT("Create window error"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
			m_IsProgramLooping = false;
		}
	}

	UnregisterClass(TEXT("m_pClassName"), hInstance); //取消注册的窗口
	return 0;
}

void GLApplication::ResizeDraw(bool bEnable)
{

}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam)
{
	LONG userData = GetWindowLong(hwnd, GWL_USERDATA);
	if (userData == 0)
	{
		//程序首次运行
		if (uMsg == WM_CREATE)
		{
			//返回窗口结构的指针，保存刚创建的程序实例的类
			CREATESTRUCT* pCreation = reinterpret_cast<CREATESTRUCT*>(lParam);
			GLApplication* pApp = reinterpret_cast<GLApplication*>(pCreation->lpCreateParams);
			SetWindowLong(hwnd, GWL_USERDATA, reinterpret_cast<LONG>(pApp));
			pApp->m_IsVisible = true;
			return 0;
		}
	} 
	else
	{
		GLApplication* pApp = reinterpret_cast<GLApplication*>(userData);
		return pApp->Message(hwnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}