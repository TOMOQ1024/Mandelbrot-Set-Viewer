#include "Menus.h"
#include "Graph.h"
#include "Color.h"
//#include <ColorDlg.h>
#include <commdlg.h>
#include <CommCtrl.h>

#pragma once
#pragma comment(lib, "ComCtl32.lib")


// メニュー 描画内容インポート
INT_PTR CALLBACK MenuImport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR input[1024] = { 0 };// 入力内容保存用の変数

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
            return (INT_PTR)IDCANCEL;
        case IDC_IMBTN:
            // 入力の決定
            GetDlgItemText(hDlg, IDC_IMIPT, (LPTSTR)input, sizeof(input) / sizeof(TCHAR));
            //	&(gdest->x0), &(gdest->y0), &(gdest->size),
            //	&(gdest->color0), &(gdest->color1), &(gdest->color2), &(gdest->color_stop0), &(gdest->color_stop1),
            //	&(gdest->color_mode), /*&(gdest->scale), */&(gdest->limit)
            if (SetGraphData(&graph, input)) {
                swprintf(input, 1024, L"%f %f", graph.x0, graph.y0);
                //swprintf(input, 1024, L"%06X %06X %06X", graph.color0, graph.color1, graph.color2);
                //swprintf(input, 1024, L"%f %u", graph.scale, graph.limit);
                //lstrcpy(input, TEXT("インポート成功"));
            }
            else {
                lstrcpy(input, TEXT("インポート失敗"));
            }
            SetDlgItemText(hDlg, IDC_IMTXT, (LPCTSTR)input);
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
    static WCHAR output[1024] = { 0x00 };// 入力内容保存用の変数
    static UINT dataSize = 0;

    HWND hWnd = NULL;
    INT w, h;
    RECT rc;
    HDC hdc = NULL;
    HDC hMemDC = NULL;
    HGDIOBJ hgdiobj = NULL;
    HBITMAP hBmp = NULL;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // 出力文字列を設定する
        GetGraphData(output, 1024);

        dataSize = lstrlen(output);

        //SetDlgItemText(hDlg, IDC_EXTXTTXT, output);
        dataSize = lstrlen(output);
        
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            if (hgdiobj != NULL)SelectObject(hMemDC, hgdiobj);
            if (hMemDC != NULL)DeleteDC(hMemDC);
            if (hBmp != NULL)DeleteObject(hBmp);
            if (hdc != NULL)ReleaseDC(hWnd, hdc);
            return (INT_PTR)TRUE;
        case IDC_EXTXTBTN:
        {
            // 文字列のコピー
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("クリップボードが開けません"));
                return (INT_PTR)FALSE;
            }

            EmptyClipboard();
            hg = GlobalAlloc(GHND | GMEM_SHARE, dataSize * 2 + 1);
            if (!hg) {
                CloseClipboard();
                return (INT_PTR)FALSE;
            }
            strMem = (CHAR*)GlobalLock(hg);
            lstrcpy((LPWSTR)strMem, output);
            GlobalUnlock(hg);
            SetClipboardData(CF_UNICODETEXT, hg);
            CloseClipboard();
            SetDlgItemText(hDlg, IDC_EXTXT, TEXT("クリップボードにコピーしました"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDC_EXBMPBTN:
        {
            // 画面のコピー
            hWnd = GetParent(hDlg);
            //hdc = GetDC(hDlg);
            hdc = GetDC(hWnd);
            if (hdc == NULL) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("hdcがNULLです"));
                return (INT_PTR)FALSE;
            }
            GetClientRect(hWnd, &rc);
            w = rc.right - rc.left;
            h = rc.bottom - rc.top;
            //w = GetDeviceCaps(hdc, HORZRES);
            //h = GetDeviceCaps(hdc, VERTRES);
            hMemDC = CreateCompatibleDC(hdc);
            if (hMemDC == NULL) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("hMemDCがNULLです"));
                return (INT_PTR)FALSE;
            }
            hBmp = CreateCompatibleBitmap(hdc, w, h);
            if (hBmp == NULL) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("hBmpがNULLです"));
                return (INT_PTR)FALSE;
            }
            hgdiobj = SelectObject(hMemDC, hBmp);
            BitBlt(hMemDC, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
            
            if (hBmp == NULL) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("ビットマップがNULLです"));
                return (INT_PTR)FALSE;
            }
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("クリップボードが開けません"));
                return (INT_PTR)FALSE;
            }
            EmptyClipboard();
            SetClipboardData(CF_BITMAP, hBmp);
            CloseClipboard();
            SetDlgItemText(hDlg, IDC_EXTXT, TEXT("クリップボードにコピーしました"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// 画像保存
VOID DlgImgSave(HWND hWnd)
{
    static OPENFILENAME ofn = { 0 };
    static TCHAR szPath[MAX_PATH] = { 0 };
    static TCHAR szFile[MAX_PATH] = { 0 };
    HANDLE hFile = NULL;
    static SYSTEMTIME stTime = { 0 };

    GetLocalTime(&stTime);

    wsprintf(
        szFile,
        L"Mandelbrot Set %04d%02d%02d%02d%02d%02d",
        stTime.wYear,
        stTime.wMonth,
        stTime.wDay,
        stTime.wHour,
        stTime.wMinute,
        stTime.wSecond
    );

    if (szPath[0] == TEXT('\0')) {
        GetCurrentDirectory(MAX_PATH, szPath);
    }
    if (ofn.lStructSize == 0) {
        ofn.lStructSize = sizeof(OPENFILENAME);
        ofn.hwndOwner = hWnd;
        ofn.lpstrInitialDir = szPath;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = MAX_PATH;
        ofn.lpstrDefExt = TEXT(".png");
        ofn.lpstrFilter = TEXT("PNG(*.png)\0*.png\0JPEG(*.jpg)\0*.jpg\0BMP(*.bmp)\0*.bmp\0");
        ofn.lpstrTitle = NULL;
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
    }
    if (GetSaveFileName(&ofn)) {
        switch (MessageBox(hWnd, szPath, TEXT("ファイル名を付けて保存"), MB_OK /*| MB_OKCANCEL*/)) {
        case IDOK:
        {
            hFile = CreateFile(
                szFile, GENERIC_WRITE, 0, NULL,
                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
            );
            if (hFile != INVALID_HANDLE_VALUE) {
                LONG lHeadSize = (sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFO));
                LONG lWidthSize = (display.width * sizeof(DWORD));
                LONG lImageSize = (lWidthSize * display.height);
                DWORD dwSize;

                // BITMAPFILEHEADERの初期化
                BITMAPFILEHEADER bmpHead = { 0 };
                bmpHead.bfType = 0x4D42;
                bmpHead.bfSize = lHeadSize + lImageSize;
                bmpHead.bfReserved1 = 0;
                bmpHead.bfReserved2 = 0;
                bmpHead.bfOffBits = lHeadSize;

                // BITMAPINFOの初期化
                BITMAPINFO bmpInfo = { 0 };
                bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
                bmpInfo.bmiHeader.biWidth = display.width;
                bmpInfo.bmiHeader.biHeight = display.height;
                bmpInfo.bmiHeader.biPlanes = 1;
                bmpInfo.bmiHeader.biBitCount = 32;
                bmpInfo.bmiHeader.biCompression = BI_RGB;
                bmpInfo.bmiHeader.biSizeImage = 0;
                bmpInfo.bmiHeader.biXPelsPerMeter = 0;
                bmpInfo.bmiHeader.biYPelsPerMeter = 0;
                bmpInfo.bmiHeader.biClrUsed = 0;
                bmpInfo.bmiHeader.biClrImportant = 0;

                // DIBセクションの作成
                LPDWORD lpPixel = NULL;
                HBITMAP hBitmap = NULL;
                HDC hSaveDC = NULL;
                hBitmap = CreateDIBSection(
                    NULL, &bmpInfo, DIB_RGB_COLORS,
                    (LPVOID*)&lpPixel, NULL, 0
                );
                hSaveDC = CreateCompatibleDC(GetDC(hWnd));
                if (hSaveDC != NULL && hBitmap != NULL)SelectObject(hSaveDC, hBitmap);
                if (hSaveDC != NULL)BitBlt(hSaveDC, 0, 0, display.width, display.height, GetDC(hWnd), 0, 0, SRCCOPY);

                // ファイルに書き込む
                WriteFile(hFile, &bmpHead, sizeof(BITMAPFILEHEADER), &dwSize, NULL);
                WriteFile(hFile, &bmpInfo, sizeof(bmpInfo), &dwSize, NULL);
                WriteFile(hFile, lpPixel, lImageSize, &dwSize, NULL);

                if(hSaveDC != NULL)DeleteDC(hSaveDC);
                if(hBitmap != NULL)DeleteObject(hBitmap);
                CloseHandle(hFile);
            }
            break;
        }
        }

    }
}



// メニュー 配色の設定
INT_PTR CALLBACK MenuSetColor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // コモンコントロールの初期化
    InitCommonControls();

    static BOOL first_paint = TRUE;

    static CHOOSECOLOR cc[6] = {{0}};
    static COLORREF CustColors[6][16] = {{0}};
    static GRAPH graph_cpy;

    HBRUSH hBrush, hOldBrush;
    HWND hCtrl;
    HDC hdc;
    static RECT rcs[2] = { 0 };


    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        //SetWindowPos(hDlg, NULL, 0, 0, 166*2, 110*2, (SWP_NOZORDER | SWP_NOOWNERZORDER));
        first_paint = TRUE;

        // graphのコピー
        CopyGraph(&graph_cpy, &graph);

        for (INT i = 0; i < 6; i++) {
            cc[i].lStructSize = sizeof(CHOOSECOLOR);
            cc[i].hwndOwner = hDlg;
            //cc[i].hwndOwner = GetParent(hDlg);
            for (INT j = 0; j < 16; j++) {
                CustColors[i][j] = 0x00000000;
            }
            cc[i].lpCustColors = CustColors[i];
            cc[i].Flags = CC_FULLOPEN | CC_RGBINIT;
            cc[i].rgbResult = InvertColor(graph_cpy.colors[i]);
        }

        for (INT i = 0; i < 5; i++) {
            SendDlgItemMessage(hDlg, IDC_SCSLIDER1 + i, TBM_SETPOS, TRUE, (LPARAM)(graph_cpy.color_stops[i] * 100));
        }

        for (int j = 0; j < 2; j++) {
            hCtrl = GetDlgItem(hDlg, IDC_SCPREVIEW0 + j);
            GetWindowRect(hCtrl, rcs + j);
        }
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        LONG k = GetWindowLong((HWND)lParam, GWL_ID);
        for (INT i = 0; i < 6; i++) {
            if (k == IDC_SCDISPLAY0 + i) {
                SetBkColor((HDC)wParam, InvertColor(graph_cpy.colors[i]));
                return (INT_PTR)GetStockObject(NULL_BRUSH);
            }
        }
        if (first_paint) {
            InvalidateRect(hDlg, NULL, FALSE);
            first_paint = FALSE;
        }
        break;
    }
    //case WM_ERASEBKGND: return (INT_PTR)TRUE;
    case WM_PAINT:
    {
        for (int j = 0; j < 2; j++) {
            hCtrl = GetDlgItem(hDlg, IDC_SCPREVIEW0 + j);
            hdc = GetDC(hCtrl);
            SelectObject(hdc, GetStockObject(NULL_PEN));
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
            hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
            for (int i = 0; i < rcs[j].right - rcs[j].left; i++) {
                DeleteObject(
                    SelectObject(hdc, hBrush = CreateSolidBrush(
                        j
                        ? InvertColor(GetColor(&graph_cpy, 1.0 * i / (rcs[j].right - rcs[j].left)))
                        : InvertColor(graph_cpy.colors[0])
                    ))
                );
                Rectangle(hdc, i, 0, i + 2, rcs[j].bottom - rcs[j].top);
            }
            DeleteObject(SelectObject(hdc, hOldBrush));
            ReleaseDC(hCtrl, hdc);
        }
        break;
    }
    case WM_HSCROLL:
    {
        for (INT i = 0; i < 5; i++) {
            if ((HWND)lParam == GetDlgItem(hDlg, IDC_SCSLIDER1 + i)) {
                graph_cpy.color_stops[i] = SendDlgItemMessage(hDlg, IDC_SCSLIDER1 + i, TBM_GETPOS, 0, 0) / 100.0;
            }
        }
        InvalidateRect(hDlg, rcs + 1, FALSE);
        break;
    }
    case WM_COMMAND:
    {
        // 色設定ボタン
        //for (INT i = 0; i < 6; i++) {
        //    if (lwp == IDC_SCBUTTON0 + i) {
        //        if (!ChooseColor(&cc[i]))return (INT_PTR)FALSE;
        //        graph_cpy.colors[i] = InvertColor(cc[i].rgbResult);
        //        //InvalidateRect(hDlg, rcs + (i ? 1 : 0), FALSE);
        //        //InvalidateRect(hDlg, NULL, FALSE);
        //        return (INT_PTR)FALSE;
        //    }
        //}

        switch (LOWORD(wParam))
        {
        case IDC_SCBUTTON0:
        case IDC_SCBUTTON1:
        case IDC_SCBUTTON2:
        case IDC_SCBUTTON3:
        case IDC_SCBUTTON4:
        case IDC_SCBUTTON5:
        {
            INT i = (INT)LOWORD(wParam) - IDC_SCBUTTON0;
            if (!ChooseColor(&cc[i]))break;
            graph_cpy.colors[i] = InvertColor(cc[i].rgbResult);
            InvalidateRect(hDlg, rcs + (i ? 1 : 0), FALSE);
            //InvalidateRect(hDlg, r, FALSE);
            break;
        }
        case IDC_SCCHECKBOX:
        {
            //graph_cpy.color_mode = LOWORD(wParam) - IDC_SCRADIO1;
            break;
        }

        case IDC_SCBUTTONOK:
            CopyGraph(&graph, &graph_cpy);
        case IDC_SCBUTTONCANCEL:
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    default: 
    {
        UINT i = message;
        i += 0;
    }
    }
    return (INT_PTR)FALSE;
}


// メニュー 最大計算回数設定
INT_PTR CALLBACK MenuSetLimit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TCHAR input[1024] = { 0 };// 入力内容保存用の変数
    static TCHAR output[1024] = { 0 };// 出力内容保存用の変数
    static TCHAR warnings[] = { TEXT("入力値が適切ではありません") };
    static INT flg = 0;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        wsprintf(input, L"現在の最大計算回数：%d", graph.limit);
        SetDlgItemText(hDlg, IDC_SLTXT1, (LPCTSTR)input);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_SLBTN:
            // 入力の決定
            GetDlgItemText(hDlg, IDC_SLIPT, (LPTSTR)input, (int)sizeof(input));

            flg = 0;
            if (input[0] == '+')flg = 1;
            else if (input[0] == '-')flg = -1;
            for (int i = flg * flg; i < (int)sizeof(input) && input[i]; i++) {
                if (!iswdigit(input[i])) {
                    SetDlgItemText(hDlg, IDC_SLTXT0, (LPCTSTR)warnings);
                    return (INT_PTR)FALSE;
                }
            }
            
            if (flg == -1)graph.limit -= _wtoi(&input[1]);
            if (flg ==  0)graph.limit = _wtoi(input);
            if (flg == +1)graph.limit += _wtoi(&input[1]);
            wsprintf(input, L"現在の最大計算回数：%d", graph.limit);
            SetDlgItemText(hDlg, IDC_SLTXT0, TEXT(""));
            SetDlgItemText(hDlg, IDC_SLTXT1, (LPCTSTR)input);
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