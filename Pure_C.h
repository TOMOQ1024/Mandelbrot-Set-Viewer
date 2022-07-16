#pragma once

#include "resource.h"
#include <wchar.h>

typedef struct {
	long double x0;
	long double y0;
	long double size;
	UINT limit;
} GRAPH;

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