#pragma once
#ifndef __GAROSU_HWND_HOST_H__
#define __GAROSU_HWND_HOST_H__

extern "C" {
	__declspec(dllexport) HWND CreateWin32Window(HINSTANCE hInstance, HWND parentHwnd, int width, int height);

	__declspec(dllexport) void DestroyWin32Window(void);
}

#endif