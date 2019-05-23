#pragma once

//includes
#include <Windows.h>//the basic include
#include <cmath>//all things that are relatable to math

typedef double fp;//a floating point precision-value, precision can be float(4 bytes), double(8 bytes, double 4 = 8), or long double(16 bytes).
typedef unsigned char byte;//this is easier to type every time.
union color//perhaps we will change the order later.
{
public:
	byte A;//the alpha component
	byte R;//the red component
	byte G;//the green component
	byte B;//the blue component
	//color(byte r, byte g, byte b) :
	//	A(0xFF), R(r), G(g), B(b) {};
};
//a window containing a viewport
class Window3D 
{
public:
	//these arrays are stored from left to right, then from top to bottom.
	fp* DepthBuffer;//an array that holds the distance to the screen for each pixel where the ray collided with a triangle.
	color* colors;//an array that holds the color of each pixel of the window.

	int Width;//the width of the window.
	int Height;//the height of the window.

	Window3D(int w, int h);

};

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
			*ArrayPtr = *Value++;//set current array member to the active byte of the value
		}
		Value -= ValueSize;
		//increment pointer: set pointer to next index
		ArrayPtr = NextPtr;
	}
}
