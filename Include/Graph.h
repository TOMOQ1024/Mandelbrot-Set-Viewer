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

// �`����e�̏o��
void GetGraphData(LPWSTR buf, size_t bufSize);

// �r�b�g�}�b�v�ւ̕`��Ɖ�ʑS�̖̂�����
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel, UINT width, UINT height);
