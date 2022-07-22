#pragma once

#include "framework.h"
#include "resource.h"

struct GRAPH{
	double x0;
	double y0;
	double size;
	double scale;
	UINT limit;
} graph;

void InitGraph(void);

//void SetGraphData(GRAPH* g, LPCWSTR input);

// 描画内容の出力
void GetGraphData(LPWSTR buf, size_t bufSize);

// ビットマップへの描画と画面全体の無効化
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel, UINT width, UINT height);
