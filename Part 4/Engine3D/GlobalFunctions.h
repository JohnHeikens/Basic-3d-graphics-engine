#pragma once

//includes
#include <Windows.h>//the basic include
#include <cmath>//all things that are relatable to math

typedef double fp;//a floating point precision-value, precision can be float(4 bytes), double(8 bytes, double 4 = 8), or long double(16 bytes).
typedef unsigned char byte;//this is easier to type every time.
constexpr int ByteToBits = 8;//there are 8 bits in a byte

struct color//perhaps we will change the order later.
{
public:
	color(byte a, byte r, byte g, byte b):A(a),R(r),G(g),B(b){}
	color(byte r, byte g, byte b) :A(0xff), R(r), G(g), B(b) {}
	color(byte grayscale) :A(0xff), R(grayscale), G(grayscale), B(grayscale) {}

	byte B;//the blue component
	byte G;//the green component
	byte R;//the red component
	byte A;//the alpha component
	//color(byte r, byte g, byte b) :
	//	A(0xFF), R(r), G(g), B(b) {};
};

#define RANDFP ((fp)rand()/RAND_MAX)

void FillArray(byte* ArrayPtr, byte* ArrayEndPtr, int Stride, int ValueSize, byte* Value);
void GetDesktopResolution(int& horizontal, int& vertical);
void Display(color* data, HDC hdc, int Width, int Height);