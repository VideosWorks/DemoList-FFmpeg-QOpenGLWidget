#ifndef _GLAPPLICATION_H
#define _GLAPPLICATION_H
#include "GLKeys.h"
#include "GLWindows.h"

class GLApplication
{
public:
	static GLApplication* Create(const char* pClassName);
	virtual ~GLApplication(){};

	int Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

protected:
	//初始化
	virtual bool Initialize() = 0;
	//反初始化
	virtual bool DeInitialize() = 0;
	//更新
	virtual void Update(DWORD dMillSeconds) = 0;
	//绘制
	virtual void Draw() = 0;

	//切换全屏模式
	void ToggleFullScreen();
	//终止应用程序
	void TerminateApplication();
	//窗口重新绘制
	void ResizeDraw(bool bEnable);

protected:
	GLWindow m_Window;
	GLKeys m_keys;
	GLApplication(const char* pClassName);

private:
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam);
	LRESULT Message(HWND hwnd, UINT uMsg, WPARAM wParam, WPARAM lParam);

	static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);
	const char* m_pClassName;
	bool m_IsProgramLooping; //程序循环标记
	bool m_CreateFullScreen;
	bool m_IsVisible;
	bool m_ResizeDraw;
	DWORD m_LastTickCount;
};
#endif