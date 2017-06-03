#include "stdafx.h"
#include "Paint.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

#define MAX_LOADSTRING 100
#define BUFFERSIZE 255

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 100, 30, 1000, 700, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

bool isDrawing = FALSE;
int currentX;
int currentY;
int lastX;
int lastY;
int type = LINE;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;
	
	switch (message)
	{
	case WM_CREATE:
		Restore_Window_Size(hWnd);
		prototypes.push_back(new My_Line);
		prototypes.push_back(new My_Rectangle);
		prototypes.push_back(new My_Ellipse);
		Read_Data();
		break;
	case WM_LBUTTONDOWN:
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		if (!isDrawing)
		{
			isDrawing = TRUE;
			currentX = x;
			currentY = y;
		}

		break;
	case WM_MOUSEMOVE:
	{
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);

		if (isDrawing)
		{
			lastX = x;
			lastY = y;
			WCHAR buffer[200];
			wsprintf(buffer, L"%d %d", x, y);
			SetWindowText(hWnd, buffer);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		
	} break;

	case WM_LBUTTONUP:
	{
		My_Shape* shape;
		shape = new My_Line;
		x = GET_X_LPARAM(lParam);
		y = GET_Y_LPARAM(lParam);
		switch (type)
		{
		case LINE:
			shape = new My_Line;
			break;
		case RECTANGLE:
			shape = new My_Rectangle;
			break;
		case ELLIPSE:
			shape = new My_Ellipse;
			break;
		}
		shape->SetData(currentX, currentY, x, y);
		shapes.push_back(shape);
		
		Write_Data(currentX, currentY, x, y);
		
		isDrawing = FALSE;

	} break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		My_Shape* shape;
		switch (wmId)
		{
		case ID_DRAW_LINE:
			shape = prototypes[LINE]->Create();
			type = LINE;
			break;
		case ID_DRAW_RECTANGLE:
			shape = prototypes[RECTANGLE]->Create();
			type = RECTANGLE;
			break;
		case ID_DRAW_ELLIPSE:
			shape = prototypes[ELLIPSE]->Create();
			type = ELLIPSE;
			break;
		case ID_FILE_NEW:
		{
			shapes.clear();
			fstream f;
			InvalidateRect(hWnd, NULL, TRUE);
			f.open("data.bin", ios::out | ios::trunc | ios::binary);
			f.close();
		}
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		
	}
	break;
	case WM_PAINT:
	{
		Save_Window_Size(hWnd);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		//HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		//SelectObject(hdc, hpen);
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right;
		int height = rect.bottom;

		HDC backbuffDC = CreateCompatibleDC(hdc);

		HBITMAP backbuffer = CreateCompatibleBitmap(hdc, width, height);

		int savedDC = SaveDC(backbuffDC);
		SelectObject(backbuffDC, backbuffer);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(backbuffDC, &rect, hBrush);
		DeleteObject(hBrush);

		for (int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->Draw(backbuffDC);
		}

		if (isDrawing)
		{
			switch (type)
			{
			case LINE:
				DrawDll::Functions::DrawLine(backbuffDC, currentX, currentY, lastX, lastY);
				break;
			case RECTANGLE:
				DrawDll::Functions::DrawRect(backbuffDC, currentX, currentY, lastX, lastY);
				break;
			case ELLIPSE:
				DrawDll::Functions::DrawEllip(backbuffDC, currentX, currentY, lastX, lastY);
				break;
			}
		}
		BitBlt(hdc, 0, 0, width, height, backbuffDC, 0, 0, SRCCOPY);
		RestoreDC(backbuffDC, savedDC);

		DeleteObject(backbuffer);
		DeleteDC(backbuffDC);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void Write_Data(int a, int b, int c, int d)
{
	write_file.open("data.bin", ios::out | ios::app | ios::binary);
	write_file.write(reinterpret_cast<char *>(&type), sizeof(type));
	write_file.write(reinterpret_cast<char *>(&a), sizeof(a));
	write_file.write(reinterpret_cast<char *>(&b), sizeof(b));
	write_file.write(reinterpret_cast<char *>(&c), sizeof(c));
	write_file.write(reinterpret_cast<char *>(&d), sizeof(d));
	write_file.close();
}

void Read_Data()
{
	int a, b, c, d = -1;
	read_file.open("data.bin", ios::in | ios::binary);
	My_Shape * shape;
	shape = new My_Line;
	int count = 0;
	if (read_file)
	{
		while (!read_file.eof())
		{
			read_file.read(reinterpret_cast<char *>(&type), sizeof(type));
			read_file.read(reinterpret_cast<char *>(&a), sizeof(a));
			read_file.read(reinterpret_cast<char *>(&b), sizeof(b));
			read_file.read(reinterpret_cast<char *>(&c), sizeof(c));
			read_file.read(reinterpret_cast<char *>(&d), sizeof(d));
			if (-1 == d)
			{
				MessageBox(NULL, L"No data saved", L"Information", NULL);
			}
			else
			{
				switch (type)
				{
				case LINE:
					shape = new My_Line;
					break;
				case RECTANGLE:
					shape = new My_Rectangle;
					break;
				case ELLIPSE:
					shape = new My_Ellipse;
					break;
				}
				shape->SetData(a, b, c, d);
				shapes.push_back(shape);
			}
		}
	}
	else
	{
		MessageBox(NULL, L"No data saved", L"Information", NULL);
	}
	read_file.close();
}

void Save_Window_Size(HWND hWnd)
{
	LPRECT main = new RECT;
	LPPOINT app = new POINT;
	WCHAR *curpath = new WCHAR[BUFFERSIZE];
	WCHAR *buff = new WCHAR[BUFFERSIZE];
	long int width, height;

	GetCurrentDirectory(BUFFERSIZE, curpath);
	wcscat_s(curpath, BUFFERSIZE, L"\\config.ini");

	GetWindowRect(hWnd, main);
	width = main->right - main->left;
	height = main->bottom - main->top;

	app->x = main->left;
	app->y = main->top;

	ClientToScreen(NULL, app);

	swprintf(buff, BUFFERSIZE, L"%d", app->x);
	WritePrivateProfileString(L"app", L"x", buff, curpath);

	swprintf(buff, BUFFERSIZE, L"%d", app->y);
	WritePrivateProfileString(L"app", L"y", buff, curpath);

	swprintf(buff, BUFFERSIZE, L"%d", width);
	WritePrivateProfileString(L"app", L"width", buff, curpath);

	swprintf(buff, BUFFERSIZE, L"%d", height);
	WritePrivateProfileString(L"app", L"height", buff, curpath);
}

void Restore_Window_Size(HWND hWnd)
{
	WCHAR *curpath = new WCHAR[BUFFERSIZE];
	WCHAR *buffer = new WCHAR[BUFFERSIZE];
	long int width, height, x, y;

	GetCurrentDirectory(BUFFERSIZE, curpath);
	wcscat_s(curpath, BUFFERSIZE, L".\\config.ini");

	GetPrivateProfileString(L"app", L"x", L"100", buffer, BUFFERSIZE, curpath);
	x = _wtoi(buffer);
	GetPrivateProfileString(L"app", L"y", L"30", buffer, BUFFERSIZE, curpath);
	y = _wtoi(buffer);
	GetPrivateProfileString(L"app", L"width", L"1000", buffer, BUFFERSIZE, curpath);
	width = _wtoi(buffer);
	GetPrivateProfileString(L"app", L"height", L"700", buffer, BUFFERSIZE, curpath);
	height = _wtoi(buffer);
	SetWindowPos(hWnd, NULL, x, y, width, height, NULL);
}