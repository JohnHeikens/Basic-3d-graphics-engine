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
	vec3 position = vec3(0, -2, 0);
	while (true)
	{
		vec3* vertices = new vec3[4]
		{
			vec3(0,0,0),
		vec3(1,0),
		vec3(0,0,1),
		vec3(1,0,1)
		};
		int* indices = new int[6]{
			0, 1, 2,
				3, 2, 1
		};
		color* colors = new color[2]{
			color(0xff,0,0),//red
			color(0,0,0xff)//blue
		};
		mat3x3 view = mat3x3(vec3(1, 1, 1));
		window->ClearDepthBuffer(INFINITY);
		window->ClearColor(color(0,0xff,0));//green
		window->DrawTriangles(vertices, indices, colors, 2, 4, 1, position, view, true);
		window->Update();
		const fp step = .01;
		if (IsPressing[VK_UP]) 
		{
			position.y += step;
		}
		if (IsPressing[VK_DOWN])
		{
			position.y -= step;
		}
		if (IsPressing[VK_LEFT])
		{
			position.x -= step;
		}
		if (IsPressing[VK_RIGHT])
		{
			position.x += step;
		}
	}
	delete window;
}
