#pragma once

#include "Menus.h"
#include "Graph.h"
#include "Color.h"
//#include <ColorDlg.h>
#include <commdlg.h>


// メニュー 描画内容インポート
INT_PTR CALLBACK MenuImport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR input[1024];// 入力内容保存用の変数

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_IMBTN:
            // 入力の決定
            GetDlgItemText(hDlg, IDC_IMIPT, (LPTSTR)input, (int)sizeof(input));
            lstrcat(input, TEXT("(インポート機能は未実装です)"));
            SetDlgItemText(hDlg, IDC_IMTXT, (LPCTSTR)input);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// メニュー 描画内容エクスポート
INT_PTR CALLBACK MenuExport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HGLOBAL hg;
    CHAR* strMem;
    static WCHAR output[256] = { 0x00 };// 入力内容保存用の変数
    static UINT dataSize = 0;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // 出力文字列を設定する
        GetGraphData(output, 256);

        dataSize = lstrlen(output);

        SetDlgItemText(hDlg, IDC_EXTXT1, output);
        dataSize = lstrlen(output);
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_EXBTN:
        {
            // 文字列のコピー
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("クリップボードを開くことができません"));
                return (INT_PTR)FALSE;
            }

            EmptyClipboard();
            hg = GlobalAlloc(GHND | GMEM_SHARE, dataSize * 2 + 1);
            if (!hg)return (INT_PTR)FALSE;
            strMem = (CHAR*)GlobalLock(hg);
            lstrcpy((LPWSTR)strMem, output);
            GlobalUnlock(hg);
            SetClipboardData(CF_UNICODETEXT, hg);
            CloseClipboard();
            SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("クリップボードにコピーしました"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// メニュー 配色の設定
INT_PTR CALLBACK MenuSetColor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static CHOOSECOLOR cc = { 0 };
    static COLORREF color = 0, CustColors[16];
    TCHAR ccode[8];

    HBRUSH hBrush, hOldBrush;
    HWND hCtrl;
    HDC hdc;
    RECT rc;


    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        //SetWindowPos(hDlg, NULL, 0, 0, 166*2, 110*2, (SWP_NOZORDER | SWP_NOOWNERZORDER));
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hDlg;
        cc.rgbResult = graph.color0;
        cc.lpCustColors = CustColors;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
        break;
    case WM_CTLCOLORSTATIC:
    {
        LONG i = GetWindowLong((HWND)lParam, GWL_ID);
        if (i == IDC_SCDISPLAY0) {
            SetBkColor((HDC)wParam, InvertColor(graph.color0));
            wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph.color0), GetGValue(graph.color0), GetRValue(graph.color0));
            SetDlgItemText(hDlg, IDC_SCCCODE0, (LPCTSTR)ccode);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i >= IDC_SCDISPLAY1 && i <= IDC_SCDISPLAY4) {
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCDISPLAY5) {
            SetBkColor((HDC)wParam, InvertColor(graph.color1));
            wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph.color1), GetGValue(graph.color1), GetRValue(graph.color1));
            SetDlgItemText(hDlg, IDC_SCCCODE1, (LPCTSTR)ccode);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCDISPLAY6) {
            SetBkColor((HDC)wParam, InvertColor(graph.color2));
            wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph.color2), GetGValue(graph.color2), GetRValue(graph.color2));
            SetDlgItemText(hDlg, IDC_SCCCODE2, (LPCTSTR)ccode);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        break;
    }
    case WM_PAINT:
    {
        for (int j = 0; j < 4; j++) {
            hCtrl = GetDlgItem(hDlg, IDC_SCDISPLAY1 + j);
            GetWindowRect(hCtrl, &rc);
            hdc = GetDC(hCtrl);
            SelectObject(hdc, GetStockObject(NULL_PEN));
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            for (int i = 0; i < rc.right - rc.left; i++) {
                DeleteObject(
                    SelectObject(hdc, hBrush = CreateSolidBrush(
                        j==3
                        ? Grad(graph.color1, graph.color2, 1.0 * i / (rc.right - rc.left))
                        : HSV(1.0 * i / (rc.right - rc.left), 1 - j / 4.0, 1)
                    ))
                );
                Rectangle(hdc, i, 0, i + 2, rc.bottom - rc.top);
            }
            SelectObject(hdc, hOldBrush);
            DeleteObject(hBrush);
            ReleaseDC(hCtrl, hdc);
        }
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SCBUTTON0:
        {
            if (!ChooseColor(&cc))return (INT_PTR)FALSE;
            graph.color0 = InvertColor(cc.rgbResult);
            //SetClassLong(hDlg, GCLP_HBRBACKGROUND,
            //    (LONG)CreateSolidBrush(cc.rgbResult));
            InvalidateRect(hDlg, NULL, FALSE);
            break;
        }
        case IDC_SCBUTTONOK:
            EndDialog(hDlg, IDOK);
            return (INT_PTR)TRUE;
        case IDC_SCBUTTONCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

/*

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);// 未使用時エラーの抑制
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

*/