// Pure_C.c : アプリケーションのエントリ ポイントを定義します。
//

#include "Graph.h"
#include "Color.h"
#include "Menus.h"
#include <omp.h>

#define MAX_LOADSTRING 100



// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// グラフ構造体
extern struct GRAPH graph;


// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // graphの初期化
    InitGraph(GRAPH_INIT_ALL);

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MANDELBROTSET, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // アクセラレータテーブルのハンドル
    // アクセラレータテーブル :
    //     特別なキーの組み合わせと発生させるIDを組み合わせたもの
    //     メニューの操作に用いる
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROTSET));

    MSG msg;

    // メイン メッセージ ループ:
    // メッセージの取得(メッセージが取得できるまで待機する)
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);// 仮想キーコードを文字に変換する
            DispatchMessage(&msg);// ウィンドウプロシージャにメッセージを渡す
        }
    }

    return (int) msg.wParam;
}



// ウィンドウ クラスの登録
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    // ウィンドウクラスのパラメータ
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);// 構造体のサイズ

    wcex.style          = CS_HREDRAW | CS_VREDRAW;// ウィンドウスタイル
    wcex.lpfnWndProc    = WndProc;// ウィンドウプロシージャ
    wcex.cbClsExtra     = 0;// ウィンドウクラス構造体の後ろに割り当てる補足バイト数
    wcex.cbWndExtra     = 0;// ウィンドウインスタンスの後ろに割り当てる補足バイト数
    wcex.hInstance      = hInstance;// このクラスのためのウィンドウプロシージャがあるインスタンスハンドル
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROTSET));// アイコンのハンドル
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);// マウスカーソルのハンドル
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);// 背景描画用ブラシのハンドル
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MANDELBROTSET);// デフォルトメニュー名
    wcex.lpszClassName  = szWindowClass;// このウィンドウクラスにつける名前
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));// 16x16の小さいサイズのアイコン

    // ウィンドウクラスの登録
    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(
       szWindowClass,       // lpWindowName : ウィンドウクラスの名前
       szTitle,             // lpWindowName : ウィンドウの名前
       WS_OVERLAPPEDWINDOW, // dwStyle      : ウィンドウのスタイル
       CW_USEDEFAULT,       // x            : 初期位置
       0,                   // y
       CW_USEDEFAULT,       // nWidth       : ウィンドウ幅
       0,                   // nHeight
       NULL,                // hWndParent   : 親ウィンドウ
       NULL,                // hMenu        : メニューのハンドル
       hInstance,           // hInstance    : インスタンスハンドル
       NULL                 // lpParam      : メッセージに渡されるパラメータ
   );

   if (!hWnd)
   {
       // ウィンドウ生成に失敗したとき，FALSEを返す
       return FALSE;
   }

   // ウィンドウの表示
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_SIZE     - リサイズ時の処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static UINT width = 0, height = 0;

    HDC hdc;// デバイスコンテキストのハンドル
    PAINTSTRUCT ps;
    static LPDWORD lpPixel;
    static BITMAPINFO bmpInfo;
    static int mx = -100, my = -100;
    BOOL m_in = FALSE;
    UINT mlen = min(width, height);

    
    switch (message)
    {
    case WM_CREATE:
    {
        width = GetSystemMetrics(SM_CXSCREEN);
        height = GetSystemMetrics(SM_CYSCREEN);
        mlen = min(width, height);
        lpPixel = (LPDWORD)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, width * height * 4);
        // DIBの情報を設定する
        bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo.bmiHeader.biPlanes = 1;
        bmpInfo.bmiHeader.biBitCount = 32;
        bmpInfo.bmiHeader.biCompression = BI_RGB;
        SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
        break;
    }
    case WM_SIZE:
    {
        mlen = min(width, height);
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
        break;
    }
    case WM_LBUTTONDOWN:
    {
        graph.x0 += (mx - (double)width / 2) / mlen * graph.size;
        graph.y0 -= (my - (double)height / 2) / mlen * graph.size;
        //graph.size *= 0.5;
        SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
        break;
        /*
        PAINTSTRUCT ps;//
        HDC hdc = BeginPaint(hWnd, &ps);// デバイスコンテキストのハンドル
        // TODO: HDC を使用する描画コードをここに追加してください...
        //TextOut(hdc, 10, 10, L"sample text", lstrlen(L"sample text"));
        UINT x, y;
        InvalidateRect(hWnd, NULL, TRUE);
        for (y = 0; y < height; y++) {
            for (x = 0; x < width; x++) {
                //cells[y][x] = ColorAt(x, y, width, height);
                SetPixel(hdc, x, y, ColorAt(x, y, width, height));
            }
        }


        wsprintf(strSize, L"width = %d : height = %d", width, height);
        TextOut(hdc, 10, 10, strSize, lstrlen(strSize));
        */
        /*
        RECT rect;
        GetClientRect(hWnd, &rect);
        SelectObject(hdc, GetStockObject(GRAY_BRUSH));
        Rectangle(hdc, 10, 10, 100, 100);
        SelectObject(hdc, GetStockObject(BLACK_BRUSH));
        SetTextColor(hdc, RGB(255, 0, 0));
        DrawText(
            hdc,
            TEXT("画面幅に合わせて改行を入れるテキスト描画のテスト"),
            -1,
            &rect,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE // | DT_WORDBREAK //
        );
        */
        /*EndPaint(hWnd, &ps);
        break;*/
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hWnd, &ps);
        // 表画面へ転送
        SetDIBitsToDevice(hdc, 0, 0, width, height, 0, 0, 0, height, lpPixel, &bmpInfo, DIB_RGB_COLORS);
        StretchDIBits(hdc, 0, 0, width, height, 0, 0, width, height, lpPixel, &bmpInfo, DIB_RGB_COLORS, SRCCOPY);
        //TCHAR str[128];
        //wsprintf(str, L"width: %d, height: %d", width, height);
        //TextOut(hdc, 10, 10, str, lstrlen(str));
        //wsprintf(str, L"mouseX: %d, mouseY: %d", mx, my);
        //TextOut(hdc, 10, 30, str, lstrlen(str));
        //swprintf_s(str, sizeof(str)/sizeof(str[0]), TEXT("%25.20f"), 2.1);
        //TextOut(hdc, 10, 50, str, lstrlen(str));
        if (m_in) {
            SelectObject(hdc, GetStockObject(WHITE_BRUSH));
            Rectangle(hdc, mx - 10, my - 10, mx + 10, my + 10);
        }
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_MOUSEHOVER:
    {
        m_in = TRUE;
        break;
    }
    case WM_MOUSELEAVE:
    {
        mx = -100;
        my = -100;
        m_in = FALSE;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int k = /*GET_KEYSTATE_WPARAM(wParam) / */GET_WHEEL_DELTA_WPARAM(wParam);
        double sc = 0 < k ? graph.scale : 1 / graph.scale;
        //graph.x0 = (graph.x0 + (sc - 1) * (graph.x0 + (mx - (double)width / 2) / mlen * graph.size)) / sc;
        graph.x0 += (sc - 1) * (mx - (double)width / 2) / mlen * graph.size / sc;
        graph.y0 -= (sc - 1) * (my - (double)height / 2) / mlen * graph.size / sc;
        //graph.y0 = (graph.y0 - (sc - 1) * (my - (double)height / 2) / mlen * graph.size) / sc;
        graph.size /= sc;
        SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
        break;
    }
    case WM_MOUSEMOVE:
    {
        // WM_MOUSELEAVEの有効化
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hWnd;
        TrackMouseEvent(&tme);
        
        // マウスカーソル位置の取得
        POINTS mp = MAKEPOINTS(lParam);
        mx = mp.x;
        my = mp.y;
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 選択されたメニューの解析:
        switch (wmId)
        {
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case IDM_IMPORT:
            if (DialogBox(hInst, MAKEINTRESOURCE(IDD_IMBOX), hWnd, MenuImport) == IDCANCEL) {
                SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
                InvalidateRect(hWnd, NULL, FALSE);
            }
            break;
        case IDM_EXPORT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_EXBOX), hWnd, MenuExport);
            break;
        case IDM_SETCOLOR:
            if (DialogBox(hInst, MAKEINTRESOURCE(IDD_SCBOX), hWnd, MenuSetColor) == IDOK) {
                SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
                InvalidateRect(hWnd, NULL, FALSE);
            }
            break;
        case IDM_SETLIMIT:
            if (DialogBox(hInst, MAKEINTRESOURCE(IDD_SLBOX), hWnd, MenuSetLimit) == IDCANCEL) {
                SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
                InvalidateRect(hWnd, NULL, FALSE);
            }
            break;
        case IDM_INITALL:
            InitGraph(GRAPH_INIT_ALL);
            SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case IDM_INITAREA:
            InitGraph(GRAPH_INIT_AREA);
            SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        case IDM_INITCOLOR:
            InitGraph(GRAPH_INIT_COLOR);
            SetBmp(hWnd, &bmpInfo, lpPixel, width, height);
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        HeapFree(GetProcessHeap(), 0, lpPixel);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



