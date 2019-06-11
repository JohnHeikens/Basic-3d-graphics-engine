#pragma once
#include "GlobalFunctions.h"
class Image
{
public:
	Image();
	Image(color* ptr, int w, int h);
	~Image();
	color* colors;
	int Width;
	int Height;
	int ChannelCount;
	inline color GetPixel(int x, int y);
	inline void SetPixel(int x, int y, color color);
	static Image FromFile(std::string path);
private:

};
