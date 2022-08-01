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
		graph.color_mode = 3;// 3: カスタマイズ
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
	gdest->color_mode = gsrc->color_mode;
}

void SetGraphData(struct GRAPH *gdest, LPCWSTR input)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!フォーマットに適合する文字列か判定!!!!!!!!!!!!!!!!!!!!!!!
	/*int i = 0, j = 0, k = 0;
	WCHAR data[4][30];
	while (input[i+j]) {
		if (input[i + j] == L'/') {
			k += 1;
			j += i;
			i = 0;
		}
		else {
			data[k][i] = input[i + j];
			i += 1;
		}
	}*/

	swscanf_s(
		input, L"%Lf/%Lf/%Lf/%lu/%lu/%lu/%Lf/%Lf/%d/%Lf/%u",
		&(gdest->x0), &(gdest->y0), &(gdest->size),
		&(gdest->color0), &(gdest->color1), &(gdest->color2), &(gdest->color_stop0), &(gdest->color_stop1),
		&(gdest->color_mode), &(gdest->scale), &(gdest->limit)
	);
}

void GetGraphData(LPWSTR buf, size_t bufSize)
{
	swprintf(
		buf, bufSize,
		L"%29.20Lf/%29.20Lf/%29.20Lf/%lu/%lu/%lu/%4.2Lf/%4.2Lf/%d/%6.4Lf/%u",
		graph.x0, graph.y0, graph.size,
		graph.color0, graph.color1, graph.color2, graph.color_stop0, graph.color_stop1,
		graph.color_mode, graph.scale, graph.limit
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
