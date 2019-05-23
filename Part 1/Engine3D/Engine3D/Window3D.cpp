#include "Window3D.h"

Window3D::Window3D(int w, int h)
{
	//create the arrays
	DepthBuffer = (fp*)calloc(w * h, sizeof(fp));
	colors = (color*)calloc(w * h, sizeof(color));
	//fill the arrays
	float MaxDistance = INFINITY;
	FillArray((byte*)DepthBuffer, (byte*)(DepthBuffer + w * h), sizeof(fp), sizeof(fp), (byte*)& MaxDistance);
	color BackgroundColor = {};
	BackgroundColor.A = (byte)0xff;
	BackgroundColor.R = (byte)0xff;//red
	BackgroundColor.G = (byte)0;
	BackgroundColor.B = (byte)0;
};
