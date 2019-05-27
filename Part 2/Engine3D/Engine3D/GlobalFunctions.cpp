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