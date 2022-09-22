#pragma once

#include "framework.h"
#include "resource.h"

#define GRAPH_INIT_AREA		0x001
#define GRAPH_INIT_COLOR	0x010
#define GRAPH_INIT_OTHER	0x100
#define GRAPH_INIT_ALL		0x111

struct GRAPH{
	double x0;
	double y0;
	double size;
	double scale;
	UINT limit;
	int color_mode;
	COLORREF color0;// 収束部
	COLORREF color1;// 発散部1
	COLORREF color2;// 発散部2
	double color_stop0;
	double color_stop1;
} graph;

void InitGraph(UINT);

// 構造体のコピー
void CopyGraph(struct GRAPH *gdest, struct GRAPH *gsrc);

void SetGraphData(struct GRAPH* gdest, LPCWSTR input);

// 描画内容の出力
void GetGraphData(LPWSTR buf, size_t bufSize);

// ビットマップへの描画と画面全体の無効化
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel, UINT width, UINT height);
