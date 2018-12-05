#include <windows.h>
#include "Example.h"
#include "BasicGraphic.h"
#include "Graphics.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	//Example* pExample = new Example("Example");
	//BasicGraphic* pExample = new BasicGraphic("BasicGraphic");
	Graphics* pExample = new Graphics("Graphics");
	if (pExample != NULL)
	{
		ret = pExample->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
		delete pExample;
	}
	else
	{
		MessageBox(HWND_DESKTOP, TEXT("error"), TEXT("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}
