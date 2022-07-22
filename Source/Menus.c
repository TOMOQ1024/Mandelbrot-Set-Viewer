#pragma once

#include "Menus.h"
#include "Graph.h"


// ���j���[ �`����e�C���|�[�g
INT_PTR CALLBACK MenuImport(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR input[1024];// ���͓��e�ۑ��p�̕ϐ�

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
            // ���͂̌���
            GetDlgItemText(hDlg, IDC_IMIPT, (LPTSTR)input, (int)sizeof(input));
            lstrcat(input, TEXT("(�C���|�[�g�@�\�͖������ł�)"));
            SetDlgItemText(hDlg, IDC_IMTXT, (LPCTSTR)input);
            //EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
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
    static WCHAR output[256] = { 0x00 };// ���͓��e�ۑ��p�̕ϐ�
    static UINT dataSize = 0;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        // �o�͕������ݒ肷��
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
            // ������̃R�s�[
            if (!OpenClipboard(hDlg)) {
                SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("�N���b�v�{�[�h���J�����Ƃ��ł��܂���"));
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
            SetDlgItemText(hDlg, IDC_EXTXT2, TEXT("�N���b�v�{�[�h�ɃR�s�[���܂���"));
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
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        //SetWindowPos(hDlg, NULL, 0, 0, 166*2, 110*2, (SWP_NOZORDER | SWP_NOOWNERZORDER));
        return (INT_PTR)TRUE;
    case WM_PAINT:
    {
        /*
        PAINTSTRUCT ps;//
        HDC hdc = BeginPaint(hDlg, &ps);
        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        SetBkColor(hdc, RGB(255, 128, 0));
        //RECT rect = { 10, 10, 30, 30 };
        //FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 0)));
        SelectObject(hdc, (HPEN)CreatePen(PS_INSIDEFRAME, 2, RGB(0x00, 0x00, 0x00)));
        SelectObject(hdc, (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x99, 0x00)));
        Rectangle(
            hdc,
            10 * GetDialogBaseUnits(),
            10 * GetDialogBaseUnits(),
            34 * GetDialogBaseUnits(),
            34 * GetDialogBaseUnits()
            // 21, 26, 43, 48
        );

        SetBkColor(hdc, RGB(0xFF, 0x99, 0x00));
        SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
        RECT rect1 = { 21, 25, 43, 47 };
        TextOut(hdc, (rect1.left + rect1.right) / 2, (rect1.top + rect1.bottom) / 2 + 8, TEXT("1"), 1);

        SelectObject(hdc, (HBRUSH)CreateSolidBrush(RGB(0xFF, 0x00, 0x99)));
        Rectangle(hdc, 21, 26+37*2, 43, 48+37*2);

        SetBkColor(hdc, RGB(0xFF, 0x00, 0x99));
        SetTextAlign(hdc, TA_CENTER | TA_BASELINE);
        RECT rect2 = { 21, 25 + 37 * 2, 43, 47 + 37 * 2 };
        TextOut(hdc, (rect2.left + rect2.right) / 2, (rect2.top + rect2.bottom) / 2 + 8, TEXT("2"), 1);
        EndPaint(hDlg, &ps);
        */
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        LONG i = GetWindowLong((HWND)lParam, GWL_ID);
        if (i == IDC_SCINDEX0) {
            //SetTextColor((HDC)wParam, RGB(0xFF, 0x00, 0x00));
            //SetBkColor((HDC)wParam, GetSysColor(COLOR_3DFACE));
            SetBkColor((HDC)wParam, RGB(0xFF, 0x00, 0x00));
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        if (i == IDC_SCINDEX1) {
            //SetTextColor((HDC)wParam, RGB(0xFF, 0x00, 0x00));
            //SetBkColor((HDC)wParam, GetSysColor(COLOR_3DFACE));
            SetBkColor((HDC)wParam, RGB(0x00, 0xFF, 0x00));
            return (INT_PTR)GetStockObject(NULL_BRUSH);
        }
        //return (LRESULT)GetStockObject(WHITE_BRUSH);
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            /*
        case IDC_SCSTATIC:
            return (INT_PTR)TRUE;
            */
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