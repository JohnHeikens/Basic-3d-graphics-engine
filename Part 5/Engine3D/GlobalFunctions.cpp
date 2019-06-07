#include "GlobalFunctions.h"
//non-class functions
//ArrayPtr: a pointer to the array
//ArrayEndPtr: a pointer to the end of the array
//stride: the step in bytes that the pointer must be incremented
//ValueSize: the size in bytes that must be inserted
//value: a pointer to the first byte of the value
void FillArray(byte* ArrayPtr, byte* ArrayEndPtr, int Stride, int ValueSize, byte* Value)
{
	while (ArrayPtr < ArrayEndPtr)//loop till the pointer is at 'ArrayEndPtr'
	{
		byte* EndPtr = ArrayPtr + ValueSize;
		byte* NextPtr = ArrayPtr + Stride;//pointer to the next index
		while (ArrayPtr < EndPtr)
		{
			*ArrayPtr++ = *Value++;//set current array member to the active byte of the value
		}
		Value -= ValueSize;
		//increment pointer: set pointer to next index
		ArrayPtr = NextPtr;
	}
}

// Get the horizontal and vertical screen sizes in pixel
void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}
//displays the raw colors on a screen.
//data: the raw colors(argb)
//hdc: the screen
void Display(color* data, HDC hdc, int Width, int Height)
{
	// Creating temp bitmap
	HBITMAP map = CreateBitmap(Width, Height,
		1, // Color Planes, unfortunately don't know what is it actually. Let it be 1
		ByteToBits * sizeof(color), // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)data); // pointer to array
// Temp HDC to copy picture
	HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	SelectObject(src, map); // Inserting picture into our temp HDC
	// Copy image from temp HDC to window
	BitBlt(hdc, // Destination
		0,  // x and
		0,  // y - upper-left corner of place, where we'd like to copy
		Width, // width of the region
		Height, // height
		src, // source
		0,   // x and
		0,   // y of upper left corner  of part of the source, from where we'd like to copy
		SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

	DeleteObject(map);
	DeleteDC(src); // Deleting temp HDC
}