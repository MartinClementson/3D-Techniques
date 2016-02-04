#include "windowInit.h"
#include "GameTimer.h"
#include <sstream>



HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return false;

	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_DESTROY:
	{

		PostQuitMessage(0);
		break;
	}

	case WM_KEYDOWN:
		switch (wParam)
		{
			case VK_ESCAPE:
				PostQuitMessage(0);
				break;
		}
	
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void CalculateFPS(HWND& window, GameTimer& time)

{
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	if ((time.TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; //fps = frameCnt / 1
		float mspf = 1000.0f / fps; //This is the miliseconds it takes to render one frame

		std::wostringstream outs;
		outs.precision(6);

		outs << L"   " << L"FPS: " << fps << L"   "
			<< L"Time Per Frame: " << mspf << L" (ms)";
		SetWindowText(window, outs.str().c_str());


		//Reset for next average
		frameCnt = 0;
		timeElapsed += 1.0f;


	}


}