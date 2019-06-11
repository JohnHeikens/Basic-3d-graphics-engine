#include "Texture.h"

//max texture size: 64x64
const int HeightIndexes[]{
	0b0,//1x1
	0b1,//2x2
	0b101,//4x4
	0b10101,//8x8
	0b1010101,//16x16
	0b101010101,//32x32
	0b10101010101,//64x64
	0b1010101010101,//128x128
};
const int BynarySequence[]
{
	1,//1
	0b10,//2
	0b100,//4
	0b1000,//8
	0b10000,//16
	0b100000,//32
	0b1000000,//64
};

//COPIES THE COLORS, NOT THE POINTER
void Texture::Load(color* colorptr, int w)
{
	res = 1;
	for (int i = 1; i < w; i *= 2) 
	{
		res++;
	}
	colors = (color*)calloc(HeightIndexes[res], sizeof(color));
	int ww = w * w;
	CopyArray((byte*)colorptr, (byte*)(colorptr + ww), (byte*)(colors + HeightIndexes[res - 1]));

	for (int i = res - 2; i >= 0; i--)
	{
		color* srcPtr = colors + HeightIndexes[i + 1];
		color* destPtr = colors + HeightIndexes[i];
		w = BynarySequence[i];
		int w2 = w + w;
		int w21 = w2 + 1;//x and y
		//average colors
		for (int y = 0; y < w; y++) 
		{
			int y2 = y + y;
			color* srcY = srcPtr + y2 * w2;
			color* destY = destPtr + y * w;
			for (int x = 0; x < w; x++)
			{
				color* srcX = srcY + x + x;
				*(destY + x) = color::Average(
									//xy
					*srcX,			//00
					*(srcX+1),		//10
					*(srcX + w2),	//01
					*(srcX + w21)	//11
				);
			}
		}
	}
}
Texture::Texture()
{

}
Texture::Texture(color* colorptr, int w)
{
	Load(colorptr, w);
}

color Texture::GetColor(const int level, fp x, fp y) const
{
	int w = BynarySequence[level];
	return *(colors + HeightIndexes[level] + //index of current level
		(int)((x - floor(x)) * w) + //x
		(int)((y - floor(y))* w) * w);//y
}
