#pragma once

#include "framework.h"
#include "resource.h"

// ダイアログのメッセージハンドラ

// 描画内容のインポートダイアログ
INT_PTR CALLBACK    MenuImport(HWND, UINT, WPARAM, LPARAM);

// 描画内容のエクスポートダイアログ
INT_PTR CALLBACK    MenuExport(HWND, UINT, WPARAM, LPARAM);

// 配色設定ダイアログ
INT_PTR CALLBACK    MenuSetColor(HWND, UINT, WPARAM, LPARAM);

// 最大計算回数設定ダイアログ
INT_PTR CALLBACK    MenuSetLimit(HWND, UINT, WPARAM, LPARAM);

// テンプレートのバージョンダイアログ
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


