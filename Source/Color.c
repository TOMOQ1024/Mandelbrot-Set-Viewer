#include "Color.h"
#include "Graph.h"

extern struct GRAPH graph;

COLORREF InvertColor(COLORREF color)
{
    return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

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

DWORD ColorAt(UINT x, UINT y, UINT width, UINT height)
{
    //if ((x - width / 2) * (x - width / 2) + (y - height / 2) * (y - height / 2) < 50)return 0x00ff0000;
    int t = Calc(x, y, width, height);
    if (t < 0) return (DWORD)graph.color0;
    //return (DWORD)((t * 4 % 128 + 64) * 0x00010100);
    //return (DWORD)HSV(t%128/128.0, 0.7, 1.0);
    return (DWORD)((t * 255 / graph.limit) * 0x00000100);
    //return (x + y) % 0x01000000;
    //return (0x01000000 - 1) * (x + y) / (width + height);
}

int Calc(UINT x, UINT y, UINT width, UINT height)
{
    UINT m = min(width, height);
    int i;
    double zr, zi, tmp, cr, ci;
    zr = zi = 0;
    cr = graph.x0 + (x - (double)width / 2) / m * graph.size;
    ci = graph.y0 + (y - (double)height / 2) / m * graph.size;
    for (i = 0; i < (int)graph.limit; i++) {
        if (zr * zr + zi * zi > 4) return i;
        tmp = zr * zr - zi * zi + cr;
        zi = 2 * zr * zi + ci;
        zr = tmp;
    }
    return -1;
}