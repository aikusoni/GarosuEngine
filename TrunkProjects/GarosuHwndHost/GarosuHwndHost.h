#pragma once
#ifndef __GAROSU_HWND_HOST_H__
#define __GAROSU_HWND_HOST_H__

extern "C" {
	__declspec(dllexport) void* CreateWin32Window(void* hInstance, void* parentHwnd, int width, int height);

	__declspec(dllexport) void DestroyWin32Window(void* hWnd);

	__declspec(dllexport) int TestVal(void);
}

#endif