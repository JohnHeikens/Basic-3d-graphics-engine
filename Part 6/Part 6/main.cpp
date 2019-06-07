#include "main.h"
#include <windows.h>
#include <iostream>
#include <cmath>
struct pixel {
	union {
		struct {
			/* 'a' unused, used for 32-bit alignment,
			 * could also be used to store pixel alpha
			 */
			unsigned char b, g, r, a;
		};
		int val;
	};
	pixel() {
		val = 0;
	}
};
// Window client size
const int width = 375;
const int height = 375;
/* Target fps, though it's hard to achieve this fps
 * without extra timer functionality unless you have
 * a powerfull processor. Raising this value will
 * increase the speed, though it will use up more CPU.
 */
const int fps = 50;
// Global Windows/Drawing variables
HBITMAP hbmp;
HWND hwnd;
HDC hdcMem;
// The window's DC
HDC wndDC;
HBITMAP hbmOld;
// Pointer to pixels (will automatically have space allocated by CreateDIBSection
pixel* pixels;
void onFrame(pixel* pixels) {
	// This is where all the drawing takes place
	pixel* p;
	// +0.005 each frame
	static float frameOffset = 0;
	float px; // % of the way across the bitmap
	float py; // % of the way down the bitmap
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			p = &pixels[y * width + x];
			px = float(x) / float(width);
			py = float(y) / float(height);
			p->r = (unsigned char)(((cos(px + frameOffset * 10) / sin(py + frameOffset)) * cos(frameOffset * 3) * 10) * 127 + 127);
			p->g = ~p->r;
			p->b = 255;
		}
	}
	frameOffset += 0.005f;
}

void MakeSurface(HWND hwnd) {
	/* Use CreateDIBSection to make a HBITMAP which can be quickly
	 * blitted to a surface while giving 100% fast access to pixels
	 * before blit.
	 */
	 // Desired bitmap properties
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Order pixels from top to bottom
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiColors[0].rgbBlue = 0;
	bmi.bmiColors[0].rgbGreen = 0;
	bmi.bmiColors[0].rgbRed = 0;
	bmi.bmiColors[0].rgbReserved = 0;
	HDC hdc = GetDC(hwnd);
	// Create DIB section to always give direct access to pixels
	hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)& pixels, NULL, 0);
	DeleteDC(hdc);
	// Give plenty of time for main thread to finish setting up
	for (int i = 0; i < 100; i++)
	{
		DoEvents();
	}
	//Sleep(50);//time??? without sleep, it finishes
	ShowWindow(hwnd, SW_SHOW);
	// Retrieve the window's DC
	wndDC = GetDC(hwnd);
	// Create DC with shared pixels to variable 'pixels'
	hdcMem = CreateCompatibleDC(hdc);
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmp);
	// Milliseconds to wait each frame
}
LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
	{
		MakeSurface(hwnd);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// Draw pixels to window when window needs repainting
		BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		SelectObject(hdcMem, hbmOld);
		DeleteDC(wndDC);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	WNDCLASSEX wc;
	MSG msg;
	// Init wc
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = CreateSolidBrush(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"animation_class";
	wc.lpszMenuName = NULL;
	wc.style = 0;
	// Register wc
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Failed to register window class.", L"Error", MB_OK);
		return 0;
	}
	// Make window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"animation_class",
		L"Animation",
		WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP | WS_CAPTION,
		300, 200, width, height,
		NULL, NULL, hInstance, NULL);
	RECT rcClient, rcWindow;
	POINT ptDiff;
	// Get window and client sizes
	GetClientRect(hwnd, &rcClient);
	GetWindowRect(hwnd, &rcWindow);
	// Find offset between window size and client size
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	// Resize client
	MoveWindow(hwnd, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, false);
	UpdateWindow(hwnd);

	while (DoEvents()) //thread begins
	{

		//Sleep(20);
		//next frame
		// Do stuff with pixels
		onFrame(pixels);
		// Draw pixels to window
		BitBlt(wndDC, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
	}
	/*while (GetMessage(&msg, 0, 0, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	return 0;
}
//process events
//return if to continue the program
bool DoEvents()
{
	MSG msg;
	BOOL result;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		result = ::GetMessage(&msg, NULL, 0, 0);
		if (result == 0) // WM_QUIT
		{
			::PostQuitMessage(msg.wParam);
			return false;
		}
		else if (result == -1)
		{
			return false;
			// Handle errors/exit application, etc.
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return true;
}