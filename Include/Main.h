#pragma once

#include "framework.h"
#include "resource.h"

typedef struct tagDISPLAY {
	UINT width;
	UINT height;
	UINT mlen;
} DISPLAY;

extern DISPLAY display;

void SetDisplay(UINT w, UINT h);
