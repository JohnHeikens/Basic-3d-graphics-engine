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

void FillArray(byte* ArrayPtr, byte* ArrayEndPtr, int Stride, int ValueSize, byte* Value);

void GetDesktopResolution(int& horizontal, int& vertical);