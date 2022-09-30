#pragma once

#include "Menus.h"
#include "Graph.h"
#include "Color.h"
//#include <ColorDlg.h>
#include <commdlg.h>
#include <CommCtrl.h>

#pragma once
#pragma comment(lib, "ComCtl32.lib")


// ���j���[ �`����e�C���|�[�g
INT_PTR CALLBACK MenuImport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR input[1024] = { 0 };// ���͓��e�ۑ��p�̕ϐ�

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
            // ���͂̌���
            GetDlgItemText(hDlg, IDC_IMIPT, (LPTSTR)input, sizeof(input) / sizeof(TCHAR));
            //	&(gdest->x0), &(gdest->y0), &(gdest->size),
            //	&(gdest->color0), &(gdest->color1), &(gdest->color2), &(gdest->color_stop0), &(gdest->color_stop1),
            //	&(gdest->color_mode), /*&(gdest->scale), */&(gdest->limit)
            if (SetGraphData(&graph, input)) {
                swprintf(input, 1024, L"%f %f", graph.x0, graph.y0);
                //swprintf(input, 1024, L"%06X %06X %06X", graph.color0, graph.color1, graph.color2);
                //swprintf(input, 1024, L"%f %u", graph.scale, graph.limit);
                //lstrcpy(input, TEXT("�C���|�[�g����"));
            }
            else {
                lstrcpy(input, TEXT("�C���|�[�g���s"));
            }
            SetDlgItemText(hDlg, IDC_IMTXT, (LPCTSTR)input);
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// ���j���[ �`����e�G�N�X�|�[�g
INT_PTR CALLBACK MenuExport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    HGLOBAL hg;
    CHAR* strMem;
    static WCHAR output[1024] = { 0x00 };// ���͓��e�ۑ��p�̕ϐ�
    static UINT dataSize = 0;

    HWND hWnd = GetParent(hDlg);
    INT w, h;
    HDC hdc = NULL;
    HDC hMemDC = NULL;
    HGDIOBJ hgdiobj = NULL;
    HBITMAP hBmp = NULL;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // �o�͕������ݒ肷��
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
            // ������̃R�s�[
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("�N���b�v�{�[�h���J���܂���"));
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
            SetDlgItemText(hDlg, IDC_EXTXT, TEXT("�N���b�v�{�[�h�ɃR�s�[���܂���"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDC_EXBMPBTN:
        {
            // ��ʂ̃R�s�[
            //hdc = GetDC(hDlg);
            hdc = GetDC(hWnd);
            w = GetDeviceCaps(hdc, HORZSIZE);
            h = GetDeviceCaps(hdc, VERTSIZE);
            hMemDC = CreateCompatibleDC(hdc);
            hBmp = CreateCompatibleBitmap(hMemDC, w, h);
            hgdiobj = SelectObject(hMemDC, hBmp);
            BitBlt(hMemDC, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
            
            if (hBmp == NULL) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("�r�b�g�}�b�v��NULL�ł�"));
                return (INT_PTR)FALSE;
            }
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT, TEXT("�N���b�v�{�[�h���J���܂���"));
                return (INT_PTR)FALSE;
            }
            EmptyClipboard();
            SetClipboardData(CF_BITMAP, hBmp);
            CloseClipboard();
            SetDlgItemText(hDlg, IDC_EXTXT, TEXT("�N���b�v�{�[�h�ɃR�s�[���܂���"));
            //SetDlgItemText(hDlg, IDC_EXTXT2, strMem);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        }
        break;
    }
    return (INT_PTR)FALSE;
}


// ���j���[ �z�F�̐ݒ�
INT_PTR CALLBACK MenuSetColor(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // �R�����R���g���[���̏�����
    InitCommonControls();

    static CHOOSECOLOR cc[3] = {{0}};
    static COLORREF CustColors[3][16];
    TCHAR ccode[8];
    static struct GRAPH graph_cpy;

    HBRUSH hBrush, hOldBrush;
    HWND hCtrl;
    HDC hdc;
    RECT rc;


    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        //SetWindowPos(hDlg, NULL, 0, 0, 166*2, 110*2, (SWP_NOZORDER | SWP_NOOWNERZORDER));

        // graph�̃R�s�[
        CopyGraph(&graph_cpy, &graph);

        for (int i = 0; i < 3; i++) {
            cc[i].lStructSize = sizeof(CHOOSECOLOR);
            cc[i].hwndOwner = hDlg;
            cc[i].lpCustColors = CustColors[i];
            cc[i].Flags = CC_FULLOPEN | CC_RGBINIT;
        }
        cc[0].rgbResult = InvertColor(graph_cpy.color0);
        cc[1].rgbResult = InvertColor(graph_cpy.color1);
        cc[2].rgbResult = InvertColor(graph_cpy.color2);

        HWND hRadio = GetDlgItem(hDlg, IDC_SCRADIO1 + graph_cpy.color_mode);
        SendMessage(hRadio, BM_SETCHECK, 1, 0);
        SendDlgItemMessage(hDlg, IDC_SCSLIDER0, TBM_SETPOS, TRUE, (LPARAM)(graph_cpy.color_stop0 * 100));
        SendDlgItemMessage(hDlg, IDC_SCSLIDER1, TBM_SETPOS, TRUE, (LPARAM)(graph_cpy.color_stop1 * 100));
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        LONG i = GetWindowLong((HWND)lParam, GWL_ID);
        if (i == IDC_SCDISPLAY0) {
            SetBkColor((HDC)wParam, InvertColor(graph_cpy.color0));
            wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph_cpy.color0), GetGValue(graph_cpy.color0), GetRValue(graph_cpy.color0));
            SetDlgItemText(hDlg, IDC_SCCCODE0, (LPCTSTR)ccode);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i >= IDC_SCDISPLAY1 && i <= IDC_SCDISPLAY4) {
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCDISPLAY5) {
            SetBkColor((HDC)wParam, InvertColor(graph_cpy.color1));
            //wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph_cpy.color1), GetGValue(graph_cpy.color1), GetRValue(graph_cpy.color1));
            //SetDlgItemText(hDlg, IDC_SCCCODE1, (LPCTSTR)ccode);
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCDISPLAY6) {
            SetBkColor((HDC)wParam, InvertColor(graph_cpy.color2));
            //wsprintf(ccode, L"#%02x%02x%02x", GetBValue(graph_cpy.color2), GetGValue(graph_cpy.color2), GetRValue(graph_cpy.color2));
            //SetDlgItemText(hDlg, IDC_SCCCODE2, (LPCTSTR)ccode);
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
                        j == 3
                        ? Grad(InvertColor(graph_cpy.color1), InvertColor(graph_cpy.color2), (1.0 * i / (rc.right - rc.left) - graph_cpy.color_stop0) / (graph_cpy.color_stop1 - graph_cpy.color_stop0))
                        : HSV(1.0 * i / (rc.right - rc.left), 1 - j / 3.0, 1)
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
    case WM_HSCROLL:
    {
        if ((HWND)lParam == GetDlgItem(hDlg, IDC_SCSLIDER0)) {
            graph_cpy.color_stop0 = SendDlgItemMessage(hDlg,IDC_SCSLIDER0, TBM_GETPOS, 0, 0) / 100.0;
        }
        if ((HWND)lParam == GetDlgItem(hDlg, IDC_SCSLIDER1)) {
            graph_cpy.color_stop1 = SendDlgItemMessage(hDlg, IDC_SCSLIDER1, TBM_GETPOS, 0, 0) / 100.0;
        }
        InvalidateRect(hDlg, NULL, FALSE);
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_SCBUTTON0:
            if (!ChooseColor(&cc[0]))return (INT_PTR)FALSE;
            graph_cpy.color0 = InvertColor(cc[0].rgbResult);
            InvalidateRect(hDlg, NULL, FALSE);
            break;
        case IDC_SCBUTTON1:
            if (!ChooseColor(&cc[1]))return (INT_PTR)FALSE;
            graph_cpy.color1 = InvertColor(cc[1].rgbResult);
            InvalidateRect(hDlg, NULL, FALSE);
            break;
        case IDC_SCBUTTON2:
            if (!ChooseColor(&cc[2]))return (INT_PTR)FALSE;
            graph_cpy.color2 = InvertColor(cc[2].rgbResult);
            InvalidateRect(hDlg, NULL, FALSE);
            break;

        case IDC_SCRADIO1:
        case IDC_SCRADIO2:
        case IDC_SCRADIO3:
        case IDC_SCRADIO4:
        {
            graph_cpy.color_mode = LOWORD(wParam) - IDC_SCRADIO1;
            break;
        }

        case IDC_SCBUTTONOK:
            EndDialog(hDlg, IDOK);
            CopyGraph(&graph, &graph_cpy);
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


// ���j���[ �ő�v�Z�񐔐ݒ�
INT_PTR CALLBACK MenuSetLimit(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static TCHAR input[1024] = { 0 };// ���͓��e�ۑ��p�̕ϐ�
    static TCHAR output[1024] = { 0 };// �o�͓��e�ۑ��p�̕ϐ�
    static TCHAR warnings[] = { TEXT("���͒l���K�؂ł͂���܂���") };
    static INT flg = 0;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        wsprintf(input, L"���݂̍ő�v�Z�񐔁F%d", graph.limit);
        SetDlgItemText(hDlg, IDC_SLTXT1, (LPCTSTR)input);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_SLBTN:
            // ���͂̌���
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
            wsprintf(input, L"���݂̍ő�v�Z�񐔁F%d", graph.limit);
            SetDlgItemText(hDlg, IDC_SLTXT0, TEXT(""));
            SetDlgItemText(hDlg, IDC_SLTXT1, (LPCTSTR)input);
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


/*

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);// ���g�p���G���[�̗}��
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