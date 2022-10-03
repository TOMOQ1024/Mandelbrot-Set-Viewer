#include "Graph.h"
#include "Color.h"

extern struct GRAPH graph;

void InitGraph(UINT flg)
{
	if (flg & GRAPH_INIT_AREA) {
		graph.x0 = graph.y0 = 0;
		graph.size = 4;
	}
	if (flg & GRAPH_INIT_COLOR) {
		graph.color0 = 0x00FFFFFF;
		graph.color1 = 0x00000000;
		graph.color2 = 0x0000FF00;
		graph.color_stop0 = 0;
		graph.color_stop1 = 1;
	}
	if (flg & GRAPH_INIT_OTHER) {
		graph.scale = 1.5;
		graph.limit = 500;
		graph.inner_color_mode = GCM_I_SOLID;//GCM_I_GRAD;
		graph.outer_color_mode = GCM_O_CUSTOM;
	}
}

void CopyGraph(struct GRAPH* gdest, struct GRAPH* gsrc)
{
	gdest->x0 = gsrc->x0;
	gdest->y0 = gsrc->y0;
	gdest->size = gsrc->size;
	gdest->color0 = gsrc->color0;
	gdest->color1 = gsrc->color1;
	gdest->color2 = gsrc->color2;
	gdest->color_stop0 = gsrc->color_stop0;
	gdest->color_stop1 = gsrc->color_stop1;
	gdest->scale = gsrc->scale;
	gdest->limit = gsrc->limit;
	gdest->inner_color_mode = gsrc->inner_color_mode;
	gdest->outer_color_mode = gsrc->outer_color_mode;
}

BOOL SetGraphData(struct GRAPH *gdest, LPCWSTR input)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!フォーマットに適合する文字列か判定!!!!!!!!!!!!!!!!!!!!!!!
	int i = 0, j = 0, k = 0;
	WCHAR data[11][50] = { {0} };
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
	if (k != 10)return FALSE;
	
	
	WCHAR names[][19] = {
		L"RE:",
		L"IM:",
		L"SIZE:",
		L"COLOR_A:",
		L"COLOR_B:",
		L"COLOR_C:",
		L"COLOR_STOP_A:",
		L"COLOR_STOP_B:",
		L"INNER_COLOR_MODE:",
		L"OUTER_COLOR_MODE:",
		L"LIMIT:",
	};

	INT boo[11] = { 0 };
	for (i = 0; i < 11; i++)boo[i] = -1;

	typedef enum tagTYPE {
		TYPE_DOUBLE,
		TYPE_COLORREF,
		TYPE_INT,
		TYPE_UINT,
	} TYPE;

	TYPE typeof[] = {
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_COLORREF,
		TYPE_DOUBLE,
		TYPE_DOUBLE,
		TYPE_INT,
		TYPE_INT,
		TYPE_UINT,
	};
	void* pointers[] = {
		&(gdest->x0),
		&(gdest->y0),
		&(gdest->size),
		&(gdest->color0),
		&(gdest->color1),
		&(gdest->color2),
		&(gdest->color_stop0),
		&(gdest->color_stop1),
		&(gdest->inner_color_mode),
		&(gdest->outer_color_mode),
		//&(gdest->scale),
		&(gdest->limit),
	};

	for (i = 0; i <= k; i++) {
		for (j = 0; j <= k; j++) {
			if (0 <= boo[j])continue;
			if (wcsncmp(names[j], data[i], lstrlen(names[j])) == 0) {
				boo[j] = i;
			}
		}
	}

	for (j = 0; j < k; j++) {
		if (boo[j] < 0)return FALSE;
	}

	/* 以下，適切な入力があったときのみ実行 */
	for (j = 0; j <= 10; j++) {
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
		L"RE:%29.20Lf/IM:%29.20Lf/SIZE:%29.20Lf/COLOR_A:%06lx/COLOR_B:%06lx/COLOR_C:%06lx/COLOR_STOP_A:%4.2Lf/COLOR_STOP_B:%4.2Lf/INNER_COLOR_MODE:%d/OUTER_COLOR_MODE:%d/LIMIT:%u",
		graph.x0, graph.y0, graph.size,
		graph.color0, graph.color1, graph.color2, graph.color_stop0, graph.color_stop1,
		graph.inner_color_mode, graph.outer_color_mode, /*graph.scale, */graph.limit
	);
}


void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel, UINT width, UINT height)
{
	int y;
	bmpInfo->bmiHeader.biWidth = width;
	bmpInfo->bmiHeader.biHeight = height;
	// 裏画面への描画
	#pragma omp parallel for
	for (y = 0; y < (int)height; y++) {
		for (int x = 0; x < (int)width; x++) {
			lpPixel[x + y * width] = ColorAt(x, y, width, height);
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
