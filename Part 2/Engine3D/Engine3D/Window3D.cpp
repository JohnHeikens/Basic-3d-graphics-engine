#include "Window3D.h"
// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
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
	color BackgroundColor = {};
	BackgroundColor.A = (byte)0xff;
	BackgroundColor.R = (byte)0xff;//red
	BackgroundColor.G = (byte)0;
	BackgroundColor.B = (byte)0;

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
}
void Window3D::Show()
{
	ShowWindow(Window, 1);
}
void Window3D::Update()
{
	MSG Msg;

	//UpdateWindow(hwnd);
		//next frame
		// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}
Window3D::~Window3D()
{
	delete[] DepthBuffer;
	delete[] colors;

}
