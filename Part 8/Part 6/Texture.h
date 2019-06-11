#include"GlobalFunctions.h"
#pragma once


//width and height MUST be a power of 2
struct Texture
{
public:
	//variables
	color* colors;
	int res;//the resolution steps of this texture
	void Load(color* colorptr, int w);
	Texture();
	Texture(color* colorptr, int w);
	color GetColor(const int level, fp x, fp y) const;
};