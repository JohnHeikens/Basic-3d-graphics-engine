#pragma once
#include <windows.h>
#include <iostream>
#include <cmath>
#include "mat3x3.h"
#include "Texture.h"
#include "image.h"
//functions
bool DoEvents();
void FillTriangle3D(fp x1, fp y1, fp d1, fp x2, fp y2, fp d2, fp x3, fp y3, fp d3, fp minx, fp maxx, color FillWith);
void FillRectangle(int x, int y, int w, int h, color FillWith);
void FillCircle(fp x, fp y, fp w, fp h, color FillWith);