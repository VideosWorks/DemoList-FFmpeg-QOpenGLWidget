#include <windows.h>
#include "Example.h"
#include "BasicGraphic.h"
#include "Graphics.h"
#include "Light.h"
#include "DisplayList.h"
#include "Texture.h"
#include "Blend.h"
#include "Fog.h"
#include "Quadrics.h"
#include "Selection.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	
	//Example* pExample = new Example("Example");
	
	//BasicGraphic* pExample = new BasicGraphic("BasicGraphic");
	
	//Graphics* pExample = new Graphics("Graphics");
	
	//Light* pExample = new Light("Light");
	
	//DisplayList* pExample = new DisplayList("DisplayList");
	
	//Texture* pExample = new Texture("Texture");

	//Blend* pExample = new Blend("Blend");
	
	//Fog* pExample = new Fog("Fog");
	
	//Quadrics* pExample = new Quadrics("Quadrics");
	
	Selection* pExample = new Selection("Selection");
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
