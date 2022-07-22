#pragma once

#include "resource.h"
#include <wchar.h>

typedef struct {
	double x0;
	double y0;
	double size;
	double scale;
	UINT limit;
} GRAPH;


// https://bluefish.orz.hm/sdoc/winprog_memo.html#%E3%82%AD%E3%83%AC%E3%82%A4%E3%81%AA%E3%82%AB%E3%83%A9%E3%83%BC%E3%83%86%E3%83%BC%E3%83%96%E3%83%AB%E3%82%92%E4%BD%9C%E3%82%8B
COLORREF HSV(double h, double s, double v)
{
	double i, f, p, q, t;

	if (s == 0)return RGB(v * 255, v * 255, v * 255);

	h *= 6;
	i = (int)h;
	f = h - i;
	p = v * (1 - s);
	q = v * (1 - (s * f));
	t = v * (1 - (s * (1 - f)));

	if (i < 1)
		return RGB(v * 255, t * 255, p * 255);
	else if (i < 2)
		return RGB(q * 255, v * 255, p * 255);
	else if (i < 3)
		return RGB(p * 255, v * 255, t * 255);
	else if (i < 4)
		return RGB(p * 255, q * 255, v * 255);
	else if (i < 5)
		return RGB(t * 255, p * 255, v * 255);
	else
		return RGB(v * 255, p * 255, q * 255);
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

void GetGraphData(GRAPH *g, LPCWSTR buf, size_t bufSize)
{
	swprintf(buf, bufSize, L"%Lf/%Lf/%Lf/%u", g->x0, g->y0, g->size, g->limit);
}

