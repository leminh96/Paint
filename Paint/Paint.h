#pragma once

#include <windowsx.h>
#include "resource.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "Draw_dll.h"

using namespace std;

#define LINE 0
#define RECTANGLE 1
#define ELLIPSE 2

class My_Shape 
{
public:
	virtual void Draw(HDC hdc) = 0;
	virtual My_Shape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d) = 0;
};

class My_Line : public My_Shape 
{
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) 
	{
		DrawDll::Functions::DrawLine(hdc, x1, y1, x2, y2);
	}

	My_Shape* Create() 
	{ 
		return new My_Line; 
	}

	void SetData(int a, int b, int c, int d) 
	{
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class My_Rectangle : public My_Shape 
{
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) 
	{
		DrawDll::Functions::DrawRect(hdc, x1, y1, x2, y2);
	}

	My_Shape* Create() 
	{ 
		return new My_Rectangle; 
	}

	void SetData(int a, int b, int c, int d) 
	{
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class My_Ellipse : public My_Shape
{
public:
	int x1;
	int x2;
	int y1;
	int y2;
public:
	void Draw(HDC hdc)
	{
		DrawDll::Functions::DrawEllip(hdc, x1, y1, x2, y2);
	}
	My_Shape* Create()
	{
		return new My_Ellipse;
	}
	void SetData(int a, int b, int c, int d)
	{
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};
vector<My_Shape*> shapes;
vector<My_Shape*> prototypes;
fstream read_file, write_file;

void Write_Data(int a, int b, int c, int d);
void Read_Data();
void Save_Window_Size(HWND hWnd);
void Restore_Window_Size(HWND hWnd);