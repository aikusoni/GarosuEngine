#pragma once

#include <Windows.h>
#include "GarosuHwndHost.h"

__declspec(dllexport) void* CreateWin32Window(void* hInstance, void* parentHwnd, int width, int height)
{
	HWND handle = CreateWindowEx(0, L"static", L"",
		WS_CHILD | WS_VISIBLE,
		0, 0,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(HWND)parentHwnd,
		0,
		(HINSTANCE)hInstance,
		0);

	return (void*)handle;
}

__declspec(dllexport) void DestroyWin32Window(void* hWnd)
{
	DestroyWindow((HWND)hWnd);
}