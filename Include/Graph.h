#pragma once

#include "Main.h"

#define GRAPH_INIT_AREA		0x001
#define GRAPH_INIT_COLOR	0x010
#define GRAPH_INIT_OTHER	0x100
#define GRAPH_INIT_ALL		0x111

typedef struct tagGRAPH {
	double x0;
	double y0;
	double size;
	double scale;
	UINT limit;
	BOOL cyclic;
	COLORREF colors[6];
	double color_stops[5];
} GRAPH;

extern GRAPH graph;

void InitGraph(UINT);

// 構造体のコピー
void CopyGraph(GRAPH *gdest, GRAPH *gsrc);

BOOL SetGraphData(GRAPH* gdest, LPCWSTR input);

// 描画内容の出力
void GetGraphData(LPWSTR buf, size_t bufSize);

// ビットマップへの描画と画面全体の無効化
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel);
