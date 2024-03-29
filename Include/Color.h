#pragma once

#include "Main.h"
#include "Graph.h"

// RGB <=> BGR
COLORREF InvertColor(COLORREF color);

// HSVからCOLORREF値に変換する関数
COLORREF HSV(double h, double s, double v);

COLORREF Grad(COLORREF c0, COLORREF c1, double t);

COLORREF GetColor(GRAPH* g, double v);

// width*heightのウィンドウにおける点(x,y)を塗る色を返す関数
DWORD ColorAt(UINT x, UINT y);

// width*heightのウィンドウにおける点(x,y)の漸化式適用回数を返す関数
int Calc(UINT x, UINT y);