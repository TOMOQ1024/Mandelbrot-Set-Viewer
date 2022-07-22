#pragma once

#include "framework.h"
#include "resource.h"

// メニューのメッセージハンドラ

// 描画内容のインポートメニュー
INT_PTR CALLBACK    MenuImport(HWND, UINT, WPARAM, LPARAM);

// 描画内容のエクスポートメニュー
INT_PTR CALLBACK    MenuExport(HWND, UINT, WPARAM, LPARAM);

// 描画内容のエクスポートメニュー
INT_PTR CALLBACK    MenuSetColor(HWND, UINT, WPARAM, LPARAM);

// テンプレートのバージョンメニュー
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


