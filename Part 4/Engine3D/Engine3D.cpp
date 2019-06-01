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
		fp y1 = RANDFP * 100;
		fp y2 = y1 + RANDFP * 50;
		fp y3 = y2 + RANDFP * 50;
		fp d1 = RANDFP * 100;
		fp d2 = RANDFP * 100;
		fp d3 = RANDFP * 100;
		fp x1 = RANDFP * 200;
		fp x2 = RANDFP * 200;
		fp x3 = RANDFP * 200;
		fp minx = x1 < x2 ? x1 < x3 ? x1 : x3 : x2 < x3 ? x2 : x3;
		fp maxx = x1 > x2 ? x1 > x3 ? x1 : x3 : x2 > x3 ? x2 : x3;
		window->FillTriangle3D(RANDFP * 200, y1, RANDFP * 200, RANDFP * 200, y2, RANDFP * 200, RANDFP * 200, y3, RANDFP * 200, minx, maxx, c);
		window->Update();
	}
	delete window;
}
