#include "stdafx.h"
#include "Draw_dll.h"

namespace DrawDll
{
	void Functions::DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
	{
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}
	void Functions::DrawRect(HDC hdc, int x1, int y1, int x2, int y2)
	{
		Rectangle(hdc, x1, y1, x2, y2);
	}
	void Functions::DrawEllip(HDC hdc, int x1, int y1, int x2, int y2)
	{
		Ellipse(hdc, x1, y1, x2, y2);
	}
}