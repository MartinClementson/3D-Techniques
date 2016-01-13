#include <stdlib.h>
#include <crtdbg.h>
#include "Linker.h"
#include "windowInit.h"
#define _CRTDBG_MAP_ALLOC
HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);





int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); 
	Engine engine(&wndHandle);

	if (wndHandle)
	{
		ShowWindow(wndHandle, nCmdShow);

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				//engine->run();
			}
		}

		DestroyWindow(wndHandle);
	}
	return (int)msg.wParam;
}


//HWND InitWindow(HINSTANCE hInstance)
//{
//	WNDCLASSEX wcex = { 0 };
//	wcex.cbSize = sizeof(WNDCLASSEX);
//	wcex.style = CS_HREDRAW | CS_VREDRAW;
//	wcex.lpfnWndProc = WndProc;
//	wcex.hInstance = hInstance;
//	wcex.lpszClassName = L"BTH_D3D_DEMO";
//	if (!RegisterClassEx(&wcex))
//		return false;
//
//	RECT rc = { 0, 0, 640, 480 };
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//
//	HWND handle = CreateWindow(
//		L"BTH_D3D_DEMO",
//		L"BTH Direct3D Demo",
//		WS_OVERLAPPEDWINDOW,
//		CW_USEDEFAULT,
//		CW_USEDEFAULT,
//		rc.right - rc.left,
//		rc.bottom - rc.top,
//		nullptr,
//		nullptr,
//		hInstance,
//		nullptr);
//
//	return handle;
//}
//
//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch (message)
//	{
//
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			break;
//	}
//
//	return DefWindowProc(hWnd, message, wParam, lParam);
//}