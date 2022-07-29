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
    TCHAR ccode0[8], ccode1[8];


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
        if (i == IDC_SCINDEX0) {
            SetBkColor((HDC)wParam, InvertColor(graph.color0));
            wsprintf(ccode0, L"#%02x%02x%02x", GetBValue(graph.color0), GetGValue(graph.color0), GetRValue(graph.color0));
            SetDlgItemText(hDlg, IDC_SCCCODE0, (LPCTSTR)ccode0);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCINDEX1) {
            SetBkColor((HDC)wParam, InvertColor(graph.color1));
            wsprintf(ccode1, L"#%02x%02x%02x", GetBValue(graph.color1), GetGValue(graph.color1), GetRValue(graph.color1));
            SetDlgItemText(hDlg, IDC_SCCCODE1, (LPCTSTR)ccode1);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
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