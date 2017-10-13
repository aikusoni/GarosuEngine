#pragma once

#include <Windows.h>
#include "GarosuHwndHost.h"

__declspec(dllexport) HWND CreateWin32Window(HINSTANCE hInstance, HWND parentHwnd, int width, int height)
{
	HWND handle = CreateWindow(L"Win32Wnd", L"",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, hInstance, 0);

	return handle;
}

__declspec(dllexport) void DestroyWin32Window(void)
{

}

__declspec(dllexport) int TestVal(void)
{
	return 123;
}