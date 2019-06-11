#include "main.h"
// Window client size
int width;
int height;
/* Target fps, though it's hard to achieve this fps
 * without extra timer functionality unless you have
 * a powerfull processor. Raising this value will
 * increase the speed, though it will use up more CPU.
 */
const int fps = 50;
// Global Windows/Drawing variables
HBITMAP hbmp = NULL;
HWND hwnd = NULL;
HDC hdcMem = NULL;
// The window's DC
HDC wndDC = NULL;
HBITMAP hbmOld = NULL;
// Pointer to colors (will automatically have space allocated by CreateDIBSection
color* colors = nullptr;
fp* depthbuffer = nullptr;
Texture tex;

void onFrame() {
	static float frameoff = 0;
	color* ptr = colors;
	for (int j = 0; j < height; j++) 
	{
		fp fy = (j + frameoff) * .01;
		for (int i = 0; i < width; i++)
		{
			fp fx = i * .01;
			*ptr++ = tex.GetColor(5, fx, fy);//32px precision
		}
	}
	frameoff += 1;
	FillCircle(RANDFP * 200, RANDFP * 200, RANDFP * 200, RANDFP * 200, color(rand() & 0xff, rand() & 0xff, rand() & 0xff));
}

void MakeSurface(HWND hwnd) {
	/* Use CreateDIBSection to make a HBITMAP which can be quickly
	 * blitted to a surface while giving 100% fast access to colors
	 * before blit.
	 */
	 // Desired bitmap properties
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFO);//sizeof(BITMAPINFO);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height; // Order colors from top to bottom
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;// width* height * 4;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrUsed = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiColors[0].rgbBlue = 0;
	bmi.bmiColors[0].rgbGreen = 0;
	bmi.bmiColors[0].rgbRed = 0;
	bmi.bmiColors[0].rgbReserved = 0;
	HDC hdc = GetDC(hwnd);
	colors = nullptr;
	// Create DIB section to always give direct access to colors
	hbmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)& colors, NULL, 0);
	depthbuffer = (fp*)calloc(width * height, sizeof(fp));
	DeleteDC(hdc);
	// Give plenty of time for main thread to finish setting up
	Sleep(50);//time??? without sleep, it finishes
	ShowWindow(hwnd, SW_SHOW);
	// Retrieve the window's DC
	wndDC = GetDC(hwnd);
	// Create DC with shared colors to variable 'colors'
	hdcMem = CreateCompatibleDC(wndDC);//HDC must be wndDC!! :)
	hbmOld = (HBITMAP)SelectObject(hdcMem, hbmp);
	// Milliseconds to wait each frame
}
LRESULT CALLBACK WndProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (msg) {
	case WM_CREATE:
	{
		MakeSurface(hwnd);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// Draw colors to window when window needs repainting
		BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
		EndPaint(hwnd, &ps);
	}
	break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
	}
	break;
	case WM_DESTROY:
	{
		SelectObject(hdcMem, hbmOld);
		DeleteDC(wndDC);
		PostQuitMessage(0);
	}
	break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	//get global vars
	GetDesktopResolution(width, height);
	Image image = Image::FromFile(std::string("up.BMP"));
	tex = Texture(image.colors, image.Width);

	WNDCLASSEX wc;
	//MSG msg;
	// Init wc
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = CreateSolidBrush(0);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"animation_class";
	wc.lpszMenuName = NULL;
	wc.style = 0;
	// Register wc
	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Failed to register window class.", L"Error", MB_OK);
		return 0;
	}
	// Make window
	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		L"animation_class",
		L"Animation",
		WS_MINIMIZEBOX | WS_SYSMENU | WS_POPUP | WS_CAPTION,
		300, 200, width, height,
		NULL, NULL, hInstance, NULL);
	RECT rcClient, rcWindow;
	POINT ptDiff;
	// Get window and client sizes
	GetClientRect(hwnd, &rcClient);
	GetWindowRect(hwnd, &rcWindow);
	// Find offset between window size and client size
	ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
	ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
	// Resize client
	MoveWindow(hwnd, rcWindow.left, rcWindow.top, width + ptDiff.x, height + ptDiff.y, false);
	UpdateWindow(hwnd);

	while (DoEvents())
	{
		color c = color(rand() % 0x100, rand() % 0x100, rand() % 0x100);
		//FillRectangle(rand() % 500, rand() % 300, rand() % 500, rand() % 300, c);
		//next frame
		// Do stuff with colors
		onFrame();
		// Draw colors to window
		BitBlt(wndDC, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);
	}
	/*while (GetMessage(&msg, 0, 0, NULL) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/
	return 0;
}
//process events
//return if to continue the program
bool DoEvents()
{
	MSG msg;
	BOOL result;

	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		result = ::GetMessage(&msg, NULL, 0, 0);
		if (result == 0) // WM_QUIT
		{
			::PostQuitMessage(msg.wParam);
			return false;
		}
		else if (result == -1)
		{
			return false;
			// Handle errors/exit application, etc.
		}
		else
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	return true;
}

void DrawTriangles(vec3* vertices, int* indices, color* colors, int TriangleCount, int VerticeCount, fp zoom, vec3 position, mat3x3 view, bool fill)
{
	vec3* vertices2D = nullptr;
	//vec3 direction = info.direction;
	//vec3 right = info.right;
	//Shader shader = todraw.shader;
	int W2 = width / 2, H2 = height / 2;
	fp Multiplier = max(W2, H2) * zoom;

	//https://en.wikipedia.org/wiki/Rotation_matrix
	view.MultiplyX(Multiplier);
	view.MultiplyZ(Multiplier);

	vertices2D = (vec3*)calloc(VerticeCount, sizeof(vec3));
	vec3* PointerVector = vertices2D;

	// vertices to 2D
	vec3* p2d = PointerVector;
	vec3* p3d = vertices;
	vec3* end = p3d + VerticeCount;
	while (p3d < end)
	{
		fp
			DX = p3d->x - position.x,
			DY = p3d->y - position.y,
			DZ = p3d->z - position.z;
		// Multiply With Multiplication Matrix
		fp maty = DX * view.XtoY + DY * view.YtoY + DZ * view.ZtoY;
		if (maty <= 0)
		{
			p2d->z = maty;
		}//checking wether distances are > 0 because you don't want to draw things behind
		else
		{

			fp matx = DX * view.XtoX + DY * view.YtoX + DZ * view.ZtoX;
			fp matz = DX * view.XtoZ + DY * view.YtoZ + DZ * view.ZtoZ;

			// Transformation To Screen
			p2d->x = (matx / maty) + W2;//convert to screen coordinates
			p2d->y = (-matz / maty) + H2;
			p2d->z = DX * DX + DY * DY + DZ * DZ;

		}
		p2d++;
		p3d++;
	}

	int pointcount = TriangleCount * 3;
	for (int i = 0; i < pointcount; i += 3)//iterate for each tri
	{
		// Get Values
		int
			ptr1 = *indices++,
			ptr2 = *indices++,
			ptr3 = *indices++;
		vec3* v1 = PointerVector + ptr1;
		vec3* v2 = PointerVector + ptr2;
		vec3* v3 = PointerVector + ptr3;
		color ActiveColor = *colors++;
		fp
			x1 = v1->x,//convert to screen coordinates
			x2 = v2->x,
			x3 = v3->x,//x3 = (outx3 / outy3) * Multiplier + W2,                                        x3 = (outx3 / outy3) * Multiplier + W2,
			y1 = v1->y,
			y2 = v2->y,
			y3 = v3->y,
			minx, maxx;

		fp distance1 = v1->z;
		fp distance2 = v2->z;
		fp distance3 = v3->z;
		if (distance1 <= 0 || distance2 <= 0 || distance3 <= 0) continue;
		// Calculate Triangle Properties
		if (x1 < x2)
		{
			minx = x1 < x3 ? x1 : x3;
			maxx = x2 > x3 ? x2 : x3;
		}
		else
		{
			minx = x2 < x3 ? x2 : x3;
			maxx = x1 > x3 ? x1 : x3;
		}
		if ((int)minx == (int)maxx || minx > width || maxx < 0) continue;
		//switch points so y1 <= y2 <= y3
		bool y1y2 = y1 > y2;
		bool y2y3 = y2 > y3;
		bool y1y3 = y1 > y3;
		if (y1y2)
		{
			if (y2y3)//total opposite
			{
				fp temp = y3;//321
				y3 = y1;
				y1 = temp;
				temp = x3;
				x3 = x1;
				x1 = temp;
				temp = distance3;
				distance3 = distance1;
				distance1 = temp;
			}
			else
			{
				if (y1y3)
				{//312
					fp temp = y1;
					y1 = y2;
					y2 = y3;
					y3 = temp;
					temp = x1;
					x1 = x2;
					x2 = x3;
					x3 = temp;
					temp = distance1;
					distance1 = distance2;
					distance2 = distance3;
					distance3 = temp;
				}
				else
				{

					fp temp = y2;//213
					y2 = y1;
					y1 = temp;
					temp = x2;
					x2 = x1;
					x1 = temp;
					temp = distance2;
					distance2 = distance1;
					distance1 = temp;
				}
			}
		}
		else if (y2y3)
		{
			if (y1y3)
			{//231
				fp temp = y1;
				y1 = y3;
				y3 = y2;
				y2 = temp;
				temp = x1;
				x1 = x3;
				x3 = x2;
				x2 = temp;
				temp = distance1;
				distance1 = distance3;
				distance3 = distance2;
				distance2 = temp;
			}
			else
			{//132
				fp temp = y2;
				y2 = y3;
				y3 = temp;
				temp = x2;
				x2 = x3;
				x3 = temp;
				temp = distance2;
				distance2 = distance3;
				distance3 = temp;
			}
		}
		if ((int)y1 == (int)y3 || y1 > height || y3 < 0) continue;

		//fp distance = udTriangle(v1, v2, v3, position);
		if (fill)
		{
			FillTriangle3D(x1, y1, distance1, x2, y2, distance2, x3, y3, distance3, minx, maxx, ActiveColor);
		}
		else
		{
			//Graphics.DrawTri(color, distance1, //DX1 * DX1 + DY1 * DY1 + DZ1 * DZ1,
			//	Pclr, x1, x2, x3, y1, y2, y3, distances, bounds, PixelWidth);
		}
	}

	//}
}

void ClearDepthBuffer(fp MaxDistance)
{
	//fill the arrays
	FillArray((byte*)depthbuffer, (byte*)(depthbuffer + width * height), sizeof(fp), sizeof(fp), (byte*)& MaxDistance);
	color BackgroundColor = color(0xff, 0, 0);//red
}

void ClearColor(color BackGroundColor)
{
	//fill the screen with the background color
	FillArray((byte*)colors, (byte*)(colors + width * height), sizeof(color), sizeof(color), (byte*)& BackGroundColor);
}

//phytagorean theorem
void FillCircle(fp x, fp y, fp w, fp h, color FillWith)
{
	int MinX = (int)x;
	int MinY = (int)y;
	int MaxX = (int)(x + w);
	int MaxY = (int)(y + h);
	fp stepX = 2.0 / w;
	fp stepY = 2.0 / h;
	fp OneEpsilon = 1.0001;
	fp yWeight = -1;
	color* ptr = colors + MinX + MinY * width;
	while (yWeight < OneEpsilon)
	{
		fp yy = yWeight * yWeight;
		fp xWeight = -1;
		color* NextPtr = ptr + width;
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

void FillRectangle(fp x, fp y, fp w, fp h, color FillWith)
{
	int ix = (int)x;
	int iy = (int)y;
	FillRectangle(ix, iy, (int)(x + w) - ix, (int)(y + w) - iy, FillWith);
}

void FillRectangle(int x, int y, int w, int h, color FillWith)
{
	color* ptr = colors + x + y * width;
	color* EndPtrY = ptr + h * width;
	while (ptr < EndPtrY)
	{
		color* NextY = ptr + width;
		color* EndPtrX = ptr + w;
		while (ptr < EndPtrX)
		{
			*ptr++ = FillWith;
		}

		ptr = NextY;//ptr+=width of screen
	}


}
//y1 < y2 < y3
void FillTriangle(fp x1, fp y1, fp x2, fp y2, fp x3, fp y3, color FillWith)
{



}

//a c-sharp method from one of my old programs
void FillTriangle3D(fp x1, fp y1, fp d1, fp x2, fp y2, fp d2, fp x3, fp y3, fp d3, fp minx, fp maxx, color FillWith)
{
	fp btop = 0 > y1 ? 0 : y1,
		bbottom = height < y3 ? height : y3,
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
		int bright = width < mostright ? width : (int)mostright;
		if (bleft < bright)
		{
			//int index = (j * w + (int)bleft) * 4;

			int EndPixelIndex = j * width + bright;
			//
			//{
			int PixelIndex = j * width + bleft;
			fp* ptr = depthbuffer + PixelIndex;
			color* colourptr = colors + PixelIndex;
			fp* endptr = depthbuffer + EndPixelIndex;
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