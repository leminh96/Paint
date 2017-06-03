#pragma once

#ifdef DRAWDLL_EXPORTS
#define DRAWDLL_API __declspec(dllexport) 
#else
#define DRAWDLL_API __declspec(dllimport) 
#endif

namespace DrawDll
{
	class Functions
	{
	public:
		static DRAWDLL_API void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
		static DRAWDLL_API void DrawRect(HDC hdc, int x1, int y1, int x2, int y2);
		static DRAWDLL_API void DrawEllip(HDC hdc, int x1, int y1, int x2, int y2);
	};
}
