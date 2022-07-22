#pragma once

#include "framework.h"
#include "resource.h"

// HSVからCOLORREF値に変換する関数
COLORREF HSV(double h, double s, double v);

// width*heightのウィンドウにおける点(x,y)を塗る色を返す関数
DWORD ColorAt(UINT x, UINT y, UINT width, UINT height);

// width*heightのウィンドウにおける点(x,y)の漸化式適用回数を返す関数
int Calc(UINT x, UINT y, UINT width, UINT height);