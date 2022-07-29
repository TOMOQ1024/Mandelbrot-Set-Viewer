#include "Graph.h"
#include "Color.h"

extern struct GRAPH graph;

void InitGraph(void)
{
	graph.x0 = graph.y0 = 0;
	graph.size = 4;
	graph.scale = 1.5;
	graph.limit = 500;
	graph.color_mode = 0;
	graph.color0 = 0x00FFFFFF;
	graph.color1 = 0x00000000;
	graph.color2 = 0x0000FF00;
}

/*
void SetGraphData(GRAPH *g, LPCWSTR input)
{
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!フォーマットに適合する文字列か判定!!!!!!!!!!!!!!!!!!!!!!!
	int i = 0, j = 0, k = 0;
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
	}
}
*/

void GetGraphData(LPWSTR buf, size_t bufSize)
{
	swprintf(buf, bufSize, L"%Lf/%Lf/%Lf/%u", graph.x0, graph.y0, graph.size, graph.limit);
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
