#include "Graph.h"
#include "Color.h"
#include <omp.h>
#define N 16

void InitGraph(UINT flg)
{
	if (flg & GRAPH_INIT_AREA) {
		graph.x0 = graph.y0 = 0;
		graph.size = 4;
	}
	if (flg & GRAPH_INIT_COLOR) {
		graph.colors[0] = 0x00FFFFFF;
		graph.colors[1] = 0x00000000;
		graph.colors[2] = 0x000080FF;
		graph.colors[3] = 0x00FF00FF;
		graph.colors[4] = 0x00FFFFFF;
		graph.colors[5] = 0x00800080;
		graph.color_stops[0] = 0.0;
		graph.color_stops[1] = 0.1;
		graph.color_stops[2] = 0.4;
		graph.color_stops[3] = 0.6;
		graph.color_stops[4] = 0.8;
	}
	if (flg & GRAPH_INIT_OTHER) {
		graph.scale = 1.5;
		graph.limit = 500;
		graph.cyclic = TRUE;
	}
}

void CopyGraph(GRAPH* gdest, GRAPH* gsrc)
{
	//memcpy_s(gdest, sizeof(GRAPH), gsrc, sizeof(GRAPH));
	gdest->x0 = gsrc->x0;
	gdest->y0 = gsrc->y0;
	gdest->size = gsrc->size;
	gdest->scale = gsrc->scale;
	gdest->limit = gsrc->limit;
	gdest->cyclic = gsrc->cyclic;
	gdest->colors[0] = gsrc->colors[0];
	gdest->colors[1] = gsrc->colors[1];
	gdest->colors[2] = gsrc->colors[2];
	gdest->colors[3] = gsrc->colors[3];
	gdest->colors[4] = gsrc->colors[4];
	gdest->colors[5] = gsrc->colors[5];
	gdest->color_stops[0] = gsrc->color_stops[0];
	gdest->color_stops[1] = gsrc->color_stops[1];
	gdest->color_stops[2] = gsrc->color_stops[2];
	gdest->color_stops[3] = gsrc->color_stops[3];
	gdest->color_stops[4] = gsrc->color_stops[4];
	//gdest->x0 = gsrc->x0;
	//gdest->y0 = gsrc->y0;
	//gdest->size = gsrc->size;
	//gdest->color0 = gsrc->color0;
	//gdest->color1 = gsrc->color1;
	//gdest->color2 = gsrc->color2;
	//gdest->color_stop0 = gsrc->color_stop0;
	//gdest->color_stop1 = gsrc->color_stop1;
	//gdest->scale = gsrc->scale;
	//gdest->limit = gsrc->limit;
	//gdest->color_mode = gsrc->color_mode;
}

BOOL SetGraphData(GRAPH *gdest, LPCWSTR input)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!フォーマットに適合する文字列か判定!!!!!!!!!!!!!!!!!!!!!!!
	INT i = 0, j = 0, k = 0;
	WCHAR data[N][50] = { {0} };
	while (input[i + j]) {
		if (input[i + j] == L'/') {
			k += 1;
			j += i + 1;
			i = 0;
		}
		else {
			data[k][i] = input[i + j];
			i += 1;
		}
	}
	//if (k != N - 1)return FALSE;
	
	
	WCHAR names[N][30] = {
		L"RE:",
		L"IM:",
		L"SIZE:",
		L"LIMIT:",
		L"CYCLIC:",
		L"COLOR_0:",
		L"COLOR_1:",
		L"COLOR_2:",
		L"COLOR_3:",
		L"COLOR_4:",
		L"COLOR_5:",
		L"CSTOP_0:",
		L"CSTOP_1:",
		L"CSTOP_2:",
		L"CSTOP_3:",
		L"CSTOP_4:"
	};

	INT boo[N] = { 0 };
	for (i = 0; i < N; i++)boo[i] = -1;

	typedef enum tagTYPE {
		TYPE_DOUBLE,
		TYPE_COLORREF,
		TYPE_INT,
		TYPE_UINT,
	} TYPE;

	TYPE typeof[N] = {
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_UINT,
		TYPE_INT,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
	};
	void* pointers[N] = {
		&(gdest->x0),
		&(gdest->y0),
		&(gdest->size),
		&(gdest->limit),
		&(gdest->cyclic),
		&(gdest->colors[0]),
		&(gdest->colors[1]),
		&(gdest->colors[2]),
		&(gdest->colors[3]),
		&(gdest->colors[4]),
		&(gdest->colors[5]),
		&(gdest->color_stops[0]),
		&(gdest->color_stops[1]),
		&(gdest->color_stops[2]),
		&(gdest->color_stops[3]),
		&(gdest->color_stops[4]),
	};

	for (i = 0; i <= k; i++) {
		for (j = 0; j < N; j++) {
			if (0 <= boo[j])continue;
			if (wcsncmp(names[j], data[i], lstrlen(names[j])) == 0) {
				boo[j] = i;
			}
		}
	}

	//for (j = 0; j < 10; j++) {
	//	if (boo[j] < 0)return FALSE;
	//}

	// /* 以下，適切な入力があったときのみ実行 */
	for (j = 0; j < N; j++) {
		if (boo[j] < 0)continue;
		switch (typeof[j]) {
		case TYPE_DOUBLE:
			swscanf_s(data[boo[j]] + lstrlen(names[j]), L"%lf", (double*)pointers[j]);
			break;
		case TYPE_COLORREF:
			swscanf_s(data[boo[j]] + lstrlen(names[j]), L"%lx", (COLORREF*)pointers[j]);
			break;
		case TYPE_INT:
			swscanf_s(data[boo[j]] + lstrlen(names[j]), L"%d", (INT*)pointers[j]);
			break;
		case TYPE_UINT:
			swscanf_s(data[boo[j]] + lstrlen(names[j]), L"%u", (UINT*)pointers[j]);
			break;
		}
	}

	return TRUE;

	//swscanf_s(
	//	input,
	//	L"%Lf/%Lf/%Lf/%lu/%lu/%lu/%Lf/%Lf/%d/%u",
	//	&(gdest->x0), &(gdest->y0), &(gdest->size),
	//	&(gdest->color0), &(gdest->color1), &(gdest->color2), &(gdest->color_stop0), &(gdest->color_stop1),
	//	&(gdest->color_mode), /*&(gdest->scale), */&(gdest->limit)
	//);
}

void GetGraphData(LPWSTR buf, size_t bufSize)
{
	swprintf(
		buf, bufSize,
		L"RE:%.20Lf/IM:%.20Lf/SIZE:%.20Lf/LIMIT:%u/CYCLIC:%d/COLOR_0:%06lx/COLOR_1:%06lx/COLOR_2:%06lx/COLOR_3:%06lx/COLOR_4:%06lx/COLOR_5:%06lx/CSTOP_0:%4.2Lf/CSTOP_1:%4.2Lf/CSTOP_2:%4.2Lf/CSTOP_3:%4.2Lf/CSTOP_4:%4.2Lf",
		graph.x0, graph.y0, graph.size, graph.limit, graph.cyclic,
		graph.colors[0], graph.colors[1], graph.colors[2],
		graph.colors[3], graph.colors[4], graph.colors[5],
		graph.color_stops[0], graph.color_stops[1], graph.color_stops[2],
		graph.color_stops[3], graph.color_stops[4]
	);
}


void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel)
{
	INT W = display.width;
	INT H = display.height;
	INT y;
	bmpInfo->bmiHeader.biWidth = display.width;
	bmpInfo->bmiHeader.biHeight = display.height;
	// 裏画面への描画
	
#pragma omp parallel for
	
	for (y = 0; y < H; y++) {
		for (int x = 0; x < W; x++) {
			lpPixel[x + y * W] = ColorAt(x, y);
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
