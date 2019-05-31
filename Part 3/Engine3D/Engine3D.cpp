// Engine3D.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Window3D.h"
int main()
{
	HINSTANCE Current = GetModuleHandle(NULL);
	int w = 0, h = 0;
	GetDesktopResolution(w, h);
	Window3D* window = new Window3D(Current, w, h);
	window->Show();
	while (true)
	{
		fp f = (fp)rand() / RAND_MAX;
		color c = color((byte)rand(), (byte)rand(), (byte)rand());
		window->FillCircle(RANDFP * 200,RANDFP * 200,RANDFP * 200, RANDFP * 200,c);
		window->Update();
	}
	delete window;
}
