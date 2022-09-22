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
	COLORREF color0;// ������
	COLORREF color1;// ���U��1
	COLORREF color2;// ���U��2
	double color_stop0;
	double color_stop1;
} graph;

void InitGraph(UINT);

// �\���̂̃R�s�[
void CopyGraph(struct GRAPH *gdest, struct GRAPH *gsrc);

void SetGraphData(struct GRAPH* gdest, LPCWSTR input);

// �`����e�̏o��
void GetGraphData(LPWSTR buf, size_t bufSize);

// �r�b�g�}�b�v�ւ̕`��Ɖ�ʑS�̖̂�����
void SetBmp(HWND hWnd, BITMAPINFO* bmpInfo, LPDWORD lpPixel, UINT width, UINT height);
