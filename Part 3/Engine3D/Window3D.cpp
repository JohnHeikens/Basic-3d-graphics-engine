#include "Window3D.h"
// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//it will go to WM_DESTROY to send a message that the window will not be redrawn
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		//break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
Window3D::Window3D(HINSTANCE hInstance,int w, int h)
{
	//create the arrays
	DepthBuffer = (fp*)calloc(w * h, sizeof(fp));
	colors = (color*)calloc(w * h, sizeof(color));
	//fill the arrays
	float MaxDistance = INFINITY;
	FillArray((byte*)DepthBuffer, (byte*)(DepthBuffer + w * h), sizeof(fp), sizeof(fp), (byte*)& MaxDistance);
	color BackgroundColor = color(0xff, 0, 0);//red
	//fill the screen with the background color
	FillArray((byte*)colors, (byte*)(colors + w * h), sizeof(color), sizeof(color), (byte*)&BackgroundColor);
	WNDCLASSEX wc;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// Step 2: Creating the Window
	Window = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"The title of my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL, NULL, hInstance, NULL);

	if (Window == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	ActiveHDC = GetDC(Window);
	Width = w;
	Height = h;
}
void Window3D::Show()
{
	ShowWindow(Window, SW_RESTORE);
}
void Window3D::Update()
{
	MSG Msg;

	//next frame
		// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	//redraw the window
	Display(colors, GetDC(Window), Width, Height);
}
Window3D::~Window3D()
{
	delete[] DepthBuffer;
	delete[] colors;

}

//phytagorean theorem
void Window3D::FillCircle(fp x, fp y, fp w, fp h, color FillWith)
{
	int MinX = (int)x;
	int MinY = (int)y;
	int MaxX = (int)(x + w);
	int MaxY = (int)(y + h);
	fp stepX = 2.0 / w;
	fp stepY = 2.0 / h;
	fp OneEpsilon = 1.0001;
	fp yWeight = -1;
	color* ptr = colors + MinX + MinY * Width;
	while (yWeight < OneEpsilon) 
	{
		fp yy = yWeight * yWeight;
		fp xWeight = -1;
		color* NextPtr = ptr + Width;
		while (xWeight < OneEpsilon)
		{
			fp DistanceFromCenter = xWeight * xWeight + yy;
			if (DistanceFromCenter < 1) 
			{
				//fill pixel
				*ptr = FillWith;
			}
			*ptr++;
			xWeight += stepX;
		}
		ptr = NextPtr;
		yWeight += stepY;
	}

}
