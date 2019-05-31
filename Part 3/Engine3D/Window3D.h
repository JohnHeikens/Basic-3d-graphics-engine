#pragma once
#include "GlobalFunctions.h"

const wchar_t g_szClassName[] = L"Window3D";

//a window containing a viewport
class Window3D
{
public:
	HWND Window;//the base window
	HDC ActiveHDC;//the drawing object

	//these arrays are stored from left to right, then from top to bottom.
	fp* DepthBuffer;//an array that holds the distance to the screen for each pixel where the ray collided with a triangle.
	color* colors;//an array that holds the color of each pixel of the window.

	int Width;//the width of the window.
	int Height;//the height of the window.

	Window3D(HINSTANCE hInstance, int w, int h);
	void Show();
	void Update();
	~Window3D();

	//graphics functions
	//2d
	void FillCircle(fp x, fp y, fp w, fp h, color FillWith);
	void DrawRectangle(fp x, fp y, fp w, fp h);
	void DrawTriangle(fp x1, fp y1, fp x2, fp y2, fp x3, fp y3);
	//3d


};