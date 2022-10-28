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

// �\���̂̃R�s�[
void CopyGraph(GRAPH *gdest, GRAPH *gsrc);

BOOL SetGraphData(GRAPH* gdest, LPCWSTR input);

// �`����e�̏o��
void GetGraphData(LPWSTR buf, size_t bufSize);

// �r�b�g�}�b�v�ւ̕`��Ɖ�ʑS�̖̂�����
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel);
