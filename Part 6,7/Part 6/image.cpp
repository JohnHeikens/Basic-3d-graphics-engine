#include "image.h"
inline Image::Image()
{

}
//COPIES THE POINTER, NOT THE COLORS
Image::Image(color* ptr, int w, int h)
{
	Width = w;
	Height = h;
	colors = ptr;
}

Image::~Image()
{
	delete[] colors;
}

inline color Image::GetPixel(int x, int y)
{
	return *(colors + x + y * Width);
}

inline void Image::SetPixel(int x, int y, color color)
{
	*(colors + x + y * Width) = color;
}

Image Image::FromFile(std::string path)
{
	int w = 0, h = 0;
	std::vector<char> data = readBMP(path, w, h);
	byte* c4 = (byte*)calloc(w * h * 4, sizeof(byte));
	InsertChannel((byte*)data.data(), (byte*)data.data() + w * h * 3, c4, 3, 4, 0xff);//fill alpha channel
	return Image((color*)c4, w, h);
}
