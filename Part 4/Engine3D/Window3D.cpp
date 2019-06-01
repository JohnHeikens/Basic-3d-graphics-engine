#include "Window3D.h"
// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		//it will go to WM_DESTROY to send a message that the window will not be redrawn
		PostQuitMessage(0);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//DestroyWindow(hwnd);
		//break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
Window3D::Window3D(HINSTANCE hInstance,int w, int h)
{
	//create the arrays
	DepthBuffer = (fp*)calloc(w * h, sizeof(fp));
	colors = (color*)calloc(w * h, sizeof(color));
	//fill the arrays
	fp MaxDistance = INFINITY;
	FillArray((byte*)DepthBuffer, (byte*)(DepthBuffer + w * h), sizeof(fp), sizeof(fp), (byte*)& MaxDistance);
	color BackgroundColor = color(0xff, 0, 0);//red
	//fill the screen with the background color
	FillArray((byte*)colors, (byte*)(colors + w * h), sizeof(color), sizeof(color), (byte*)&BackgroundColor);
	WNDCLASSEX wc;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// Step 2: Creating the Window
	Window = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"The title of my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, w, h,
		NULL, NULL, hInstance, NULL);

	if (Window == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}
	ActiveHDC = GetDC(Window);
	Width = w;
	Height = h;
}
void Window3D::Show()
{
	ShowWindow(Window, SW_RESTORE);
}
void Window3D::Update()
{
	MSG Msg;

	//next frame
		// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	//redraw the window
	Display(colors, GetDC(Window), Width, Height);
}
Window3D::~Window3D()
{
	delete[] DepthBuffer;
	delete[] colors;

}

//phytagorean theorem
void Window3D::FillCircle(fp x, fp y, fp w, fp h, color FillWith)
{
	int MinX = (int)x;
	int MinY = (int)y;
	int MaxX = (int)(x + w);
	int MaxY = (int)(y + h);
	fp stepX = 2.0 / w;
	fp stepY = 2.0 / h;
	fp OneEpsilon = 1.0001;
	fp yWeight = -1;
	color* ptr = colors + MinX + MinY * Width;
	while (yWeight < OneEpsilon) 
	{
		fp yy = yWeight * yWeight;
		fp xWeight = -1;
		color* NextPtr = ptr + Width;
		while (xWeight < OneEpsilon)
		{
			fp DistanceFromCenter = xWeight * xWeight + yy;
			if (DistanceFromCenter < 1) 
			{
				//fill pixel
				*ptr = FillWith;
			}
			*ptr++;
			xWeight += stepX;
		}
		ptr = NextPtr;
		yWeight += stepY;
	}

}

void Window3D::FillRectangle(fp x, fp y, fp w, fp h, color FillWith)
{
	int ix = (int)x;
	int iy = (int)y;
	FillRectangle(ix, iy, (int)(x + w) - ix, (int)(y + w) - iy, FillWith);
}

void Window3D::FillRectangle(int x, int y, int w, int h, color FillWith)
{
	color* ptr = colors + x + y * Width;
	color* EndPtrY = ptr + h * Width;
	while (ptr < EndPtrY) 
	{
		color* NextY = ptr + Width;
		color* EndPtrX = ptr + w;
		while (ptr < EndPtrX)
		{
			*ptr++ = FillWith;
		}

		ptr = NextY;//ptr+=width of screen
	}


}
//y1 < y2 < y3
void Window3D::FillTriangle(fp x1, fp y1, fp x2, fp y2, fp x3, fp y3, color FillWith)
{



}

//a c-sharp method from one of my old programs
void Window3D::FillTriangle3D(fp x1, fp y1, fp d1, fp x2, fp y2, fp d2, fp x3, fp y3, fp d3, fp minx, fp maxx, color FillWith)
{
	fp btop = 0 > y1 ? 0 : y1,
		bbottom = Height < y3 ? Height : y3,
		//https://codeplea.com/triangular-interpolation
		x1min3 = x1 - x3,
		x2min1 = x2 - x1,
		x3min2 = x3 - x2,
		x3min1 = x3 - x1,
		y1min2 = y1 - y2,
		y1min3 = y1 - y3,
		y2min1 = y2 - y1,
		y2min3 = y2 - y3,
		y3min1 = y3 - y1,
		y3min2 = y3 - y2,
		det = y2min3 * x1min3 + x3min2 * -y3min1,

		//making a triangular distance formula
		w1 = (y2min3 * -x3 + x3min2 * -y3) / det,
		w2 = (y3min1 * -x3 + x1min3 * -y3) / det,
		v00 = w1 * d1 + w2 * d2 + (1 - w1 - w2) * d3;//at pixel x0 y0
	w1 += y2min3 / det;//w1 = (y2min3 * (1 - x3) + x3min2 * -y3) / det;
	w2 += y3min1 / det;
	fp v10 = w1 * d1 + w2 * d2 + (1 - w1 - w2) * d3,//at pixel x1 y0
		distx = v10 - v00;//per pixel to the right difference
	w1 = (y2min3 * -x3 + x3min2 * (1 - y3)) / det;
	w2 = (y3min1 * -x3 + x1min3 * (1 - y3)) / det;
	fp v01 = w1 * d1 + w2 * d2 + (1 - w1 - w2) * d3,//at pixel x0 y0
		disty = v01 - v00;//per pixel to the bottom difference
	bool
		l12w0 = x1 == x2,
		l23w0 = x2 == x3,
		l31w0 = x3 == x1;
	fp
		l12dydx = y2min1 / x2min1,
		l12yatx0 = y1 - l12dydx * x1,
		l23dydx = y3min2 / x3min2,
		l23yatx0 = y2 - l23dydx * x2,
		l31dydx = y3min1 / x3min1,
		l31yatx0 = y1 - l31dydx * x1;
	for (int j = (int)btop; j < bbottom; j++)
	{
		fp mostleft = maxx, mostright = minx;
		if (j >= y1 && j < y2)
		{
			mostleft = l12w0 ? x1 : (j - l12yatx0) / l12dydx;//(y-b)/a intersection point
		}
		if (j >= y2 && j < y3)
		{
			fp intersectx = l23w0 ? x2 : (j - l23yatx0) / l23dydx;//(y-b)/a
			if (intersectx < mostleft)
			{
				mostright = mostleft;
				mostleft = intersectx;
			}
			else
				mostright = intersectx;
		}
		if (j >= y1 && j < y3)
		{
			fp intersectx = l31w0 ? x3 : (j - l31yatx0) / l31dydx;//(y-b)/a
			if (intersectx < mostleft)
			{
				mostright = mostleft;
				mostleft = intersectx;
			}
			else mostright = intersectx;
		}

		int bleft = 0 > mostleft ? 0 : (int)mostleft;
		int bright = Width < mostright ? Width : (int)mostright;
		if (bleft < bright)
		{
			//int index = (j * w + (int)bleft) * 4;

			int EndPixelIndex = j * Width + bright;
			//
			//{
			int PixelIndex = j * Width + bleft;
			fp* ptr = DepthBuffer + PixelIndex;
			color* colourptr = colors + PixelIndex;
			fp* endptr = DepthBuffer + EndPixelIndex;
			fp* endptr2 = endptr - 2 - 1;
			fp distance = v00 + j * disty + bleft * distx;
			ptr--;
			while (ptr < endptr2)
			{
				distance += distx;
				if (*++ptr > distance)
				{
					*colourptr = FillWith;
					*ptr = distance;
				}
				colourptr++;
				distance += distx;
				if (*++ptr > distance)
				{
					*colourptr = FillWith;
					*ptr = distance;
				}
				colourptr++;
			}
			while (++ptr < endptr)
			{
				distance += distx;
				if (*ptr > distance)
				{
					*colourptr = FillWith;
					*ptr = distance;
				}
				colourptr++;
			}
		}
	}
}