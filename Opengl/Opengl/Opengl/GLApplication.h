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
	//��ʼ��
	virtual bool Initialize() = 0;
	//����ʼ��
	virtual bool DeInitialize() = 0;
	//����
	virtual void Update(DWORD dMillSeconds) = 0;
	//����
	virtual void Draw() = 0;

	//�л�ȫ��ģʽ
	void ToggleFullScreen();
	//��ֹӦ�ó���
	void TerminateApplication();
	//�������»���
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
	bool m_IsProgramLooping; //����ѭ�����
	bool m_CreateFullScreen;
	bool m_IsVisible;
	bool m_ResizeDraw;
	DWORD m_LastTickCount;
};
#endif