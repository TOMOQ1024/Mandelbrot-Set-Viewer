#pragma once

#include "framework.h"
#include "resource.h"

// HSV����COLORREF�l�ɕϊ�����֐�
COLORREF HSV(double h, double s, double v);

// width*height�̃E�B���h�E�ɂ�����_(x,y)��h��F��Ԃ��֐�
DWORD ColorAt(UINT x, UINT y, UINT width, UINT height);

// width*height�̃E�B���h�E�ɂ�����_(x,y)�̑Q�����K�p�񐔂�Ԃ��֐�
int Calc(UINT x, UINT y, UINT width, UINT height);