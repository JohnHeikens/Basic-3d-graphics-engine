#pragma once

//includes
#include <Windows.h>//the basic include
#include <cmath>//all things that are relatable to math
#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>

typedef double fp;//a floating point precision-value, precision can be float(4 bytes), double(8 bytes, double 4 = 8), or long double(16 bytes).
typedef unsigned char byte;//this is easier to type every time.
constexpr int ByteToBits = 8;//there are 8 bits in a byte
struct color//perhaps we will change the order later.
{
	union {
		struct {
			byte b;//the blue component
			byte g;//the green component
			byte r;//the red component
			byte a;//the alpha component
		};
		int val;//used for fast memory movement
	};


public:
	color(byte a, byte r, byte g, byte b) :a(a), r(r), g(g), b(b) {}
	color(byte r, byte g, byte b) :a(0xff), r(r), g(g), b(b) {}
	color(byte grayscale) :a(0xff), r(grayscale), g(grayscale), b(grayscale) {}
	color():val(0) {}
	static color Average(color c1, color c2, color c3, color c4);
};

#define RANDFP ((fp)rand()/RAND_MAX)
//global functions:

void FillArray(byte* ArrayPtr, byte* ArrayEndPtr, int Stride, int ValueSize, byte* Value);
void GetDesktopResolution(int& horizontal, int& vertical);
void CopyArray(byte* srcPtr, byte* srcEndPtr, byte* DestPtr);
std::vector<char> readBMP(const std::string& file, int& width, int& height);
void InsertChannel(byte* arrayPtr, byte* ArrayEndPtr, byte* newarrayPtr, int start, int stride, byte value);