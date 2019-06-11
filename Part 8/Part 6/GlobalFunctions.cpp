#include "GlobalFunctions.h"

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
//copies an array from src to dest
void CopyArray(byte* srcPtr, byte* srcEndPtr, byte* DestPtr) //valid
{
	while (srcPtr < srcEndPtr)
	{
		*DestPtr++ = *srcPtr++;
	}
}
//returns the average of 4 colors.
color color::Average(color c1, color c2, color c3, color c4)
{
	return color(
		((int)c1.a + c2.a + c3.a + c4.a) / 4,
		((int)c1.r + c2.r + c3.r + c4.r) / 4,
		((int)c1.g + c2.g + c3.g + c4.g) / 4,
		((int)c1.b + c2.b + c3.b + c4.b) / 4
	);
}
//source:
//https://stackoverflow.com/questions/9296059/read-pixel-value-in-bmp-file
std::vector<char> readBMP(const std::string& file, int& width, int& height)
{
	static constexpr size_t HEADER_SIZE = 54;

	std::ifstream bmp(file, std::ios::binary);

	std::array<char, HEADER_SIZE> header;
	bmp.read(header.data(), header.size());

	auto fileSize = *reinterpret_cast<uint32_t*>(&header[2]);
	auto dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
	width = *reinterpret_cast<uint32_t*>(&header[18]);
	height = *reinterpret_cast<uint32_t*>(&header[22]);
	auto depth = *reinterpret_cast<uint16_t*>(&header[28]);

	std::vector<char> img(dataOffset - HEADER_SIZE);
	bmp.read(img.data(), img.size());

	auto dataSize = ((width * 3 + 3) & (~3)) * height;
	img.resize(dataSize);
	bmp.read(img.data(), img.size());

	//switch red and blue
	/*char temp = 0;

	for (auto i = dataSize - 4; i >= 0; i -= 3)
	{
		temp = img[i];
		img[i] = img[i + 2];
		img[i + 2] = temp;

		//std::cout << "R: " << int(img[i] & 0xff) << " G: " << int(img[i + 1] & 0xff) << " B: " << int(img[i + 2] & 0xff) << std::endl;
	}*/

	return img;
}
void InsertChannel(byte* arrayPtr, byte* ArrayEndPtr, byte* newarrayPtr, int start, int stride, byte value)
{
	int i = 0;
	while (arrayPtr < ArrayEndPtr) 
	{
		if ((i++) % stride == start)
		{
			*newarrayPtr++ = value;
		}
		else 
		{
			*newarrayPtr++ = *arrayPtr++;
		}
	}
}