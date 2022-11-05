#include "Color.h"
#include "Graph.h"
#include <math.h>

#define fAbs(a) (a<-a ? -a : a)

COLORREF InvertColor(COLORREF color)
{
    return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

// https://bluefish.orz.hm/sdoc/winprog_memo.html#%E3%82%AD%E3%83%AC%E3%82%A4%E3%81%AA%E3%82%AB%E3%83%A9%E3%83%BC%E3%83%86%E3%83%BC%E3%83%96%E3%83%AB%E3%82%92%E4%BD%9C%E3%82%8B
COLORREF HSV(double h, double s, double v)
{
    if (h < 0)return 0;
    double i, f, p, q, t;

    if (s == 0)return RGB(v * 255, v * 255, v * 255);

    while (1 <= h)h -= 1;

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

COLORREF Grad(COLORREF c0, COLORREF c1, double t)
{
    if (t < 0)return c0;
    if (1 < t)return c1;
    return RGB(
        GetRValue(c0) * (1 - t) + GetRValue(c1) * t,
        GetGValue(c0) * (1 - t) + GetGValue(c1) * t,
        GetBValue(c0) * (1 - t) + GetBValue(c1) * t
    );
}

// 0 <= v <= 1
COLORREF GetColor(GRAPH* g, double v)
{
    COLORREF tmp_cr = 0;
    double tmp_lf = 0;
    double cstops[7] = { 0 };
    COLORREF colors[7] = { 0 };

    // graphÇÃì«Ç›éÊÇË
    for (INT i = 0; i < 5; i++) {
        cstops[i + 1] = g->color_stops[i];
        colors[i + 1] = g->colors[i + 1];
    }

    // cstops, colors 1~5 ÇÃÉ\Å[Ég
    for (INT i = 2; i < 6; i++) {
        for (INT j = 1; j < i; j++) {
            if (cstops[i] < cstops[j]) {
                tmp_lf = cstops[i];
                cstops[i] = cstops[j];
                cstops[j] = tmp_lf;
                tmp_cr = colors[i];
                colors[i] = colors[j];
                colors[j] = tmp_cr;
            }
        }
    }

    if (g->cyclic) {
        // èzä¬
        cstops[0] = cstops[5] - 1;
        colors[0] = colors[5];
        cstops[6] = cstops[1] + 1;
        colors[6] = colors[1];
    }
    else {
        // îÒèzä¬
        cstops[0] = 0;
        colors[0] = colors[1];
        cstops[6] = 1;
        colors[6] = colors[5];
    }
    for (INT i = 0; i < 6; i++) {
        if (v < cstops[i + 1]) {
            if (fAbs(cstops[i + 1] - cstops[i]) < 1e-10)continue;
            return RGB(
                (GetRValue(colors[i + 1]) - GetRValue(colors[i])) * (v - cstops[i]) / (cstops[i + 1] - cstops[i]) + GetRValue(colors[i]),
                (GetGValue(colors[i + 1]) - GetGValue(colors[i])) * (v - cstops[i]) / (cstops[i + 1] - cstops[i]) + GetGValue(colors[i]),
                (GetBValue(colors[i + 1]) - GetBValue(colors[i])) * (v - cstops[i]) / (cstops[i + 1] - cstops[i]) + GetBValue(colors[i])
                );
        }
    }
    return 0;
}

DWORD ColorAt(UINT x, UINT y)
{
    //if ((x - width / 2) * (x - width / 2) + (y - height / 2) * (y - height / 2) < 50)return 0x00ff0000;
    int t = Calc(x, y);
    if (t < 0) return (DWORD)graph.colors[0];
    //return (DWORD)((t * 4 % 128 + 64) * 0x00010100);
    //return (DWORD)HSV(t%128/128.0, 0.7, 1.0);
    if (graph.cyclic) {
        return GetColor(&graph, fmod(t/100.0, 1));
    }
    else {
        return GetColor(&graph, 1.0 * t / graph.limit);
    }
}

int Calc(UINT x, UINT y)
{
    int i;
    double zr, zi, tmp, cr, ci;
    zr = zi = 0;
    cr = graph.x0 + (x - (double)display.width / 2) / display.mlen * graph.size;
    ci = graph.y0 + (y - (double)display.height / 2) / display.mlen * graph.size;
    for (i = 0; i <= (int)graph.limit; i++) {
        //if (zr * zr * zr * zr + zi * zi * zi * zi> 4) return i;
        //tmp = zr * zr * zr * zr - zi * zi * zi *zi + cr;
        if (zr * zr + zi * zi> 4) return i;
        tmp = zr * zr - zi * zi + cr;
        zi = 2 * zr * zi + ci;
        zr = tmp;
        //tmp = (zr * zr - zi * zi) * zr - (2 * zr * zi + ci) * zi + cr;
        //zi = 2 * (zr * zr - zi * zi) * (2 * zr * zi + ci) + ci;
        //zr = tmp;
    }
    return -1;
}