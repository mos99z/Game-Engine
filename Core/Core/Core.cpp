// Core.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Core.h"
#include <Renderer.h>


HWND whd;

#define MAX_LOADSTRING 100
char bitFlags = 0;
bool b_FullScreenHotKey = false;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	srand((unsigned int)time(0));

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CORE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CORE));

	MSG msg;





	RendererD3D::Renderer render;
	render.Initialize(whd, 800, 600);
	FLOAT clearColor[4]{ 1.0f,0.0f,1.0f,1.0f };
	render.ClearRenderTarget(clearColor);
	render.Present();
	RendererD3D::CSharpDLLTest();
	while (true)
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			clearColor[1] = 1.0f;
			render.SetResolution(1024, 800);
			render.ClearRenderTarget(clearColor);
			render.Present();
		}
		if (GetAsyncKeyState('K'))
		{
			break;
		}
	}
	
	render.Shutdown();

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CORE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CORE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	RECT windowSize = { 0, 0, BUFFER_WIDTH, BUFFER_HEIGHT };
	HDC hdc = GetWindowDC(NULL);
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		((GetDeviceCaps(hdc, HORZRES) - BUFFER_WIDTH) >> 1), ((GetDeviceCaps(hdc, VERTRES) - BUFFER_HEIGHT) >> 1), BUFFER_WIDTH, BUFFER_HEIGHT, nullptr, nullptr, hInstance, nullptr);
	whd = hWnd;
	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

void FullScreenWindow(HWND hWnd)
{
	DWORD currStyle = GetWindowLong(hWnd, GWL_STYLE);
	HDC hdc = GetWindowDC(NULL);
	if (bitFlags & FULLSCREEN)
	{
		SetWindowLong(hWnd, GWL_STYLE, currStyle & ~STYLEMOD);
		SetWindowPos(hWnd, 0, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
	else
	{
		SetWindowLong(hWnd, GWL_STYLE, currStyle & STYLEMOD);
		SetWindowPos(hWnd, 0, ((GetDeviceCaps(hdc, HORZRES) - BUFFER_WIDTH) >> 1), ((GetDeviceCaps(hdc, VERTRES) - BUFFER_HEIGHT) >> 1), BUFFER_WIDTH, BUFFER_HEIGHT, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	}
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
	if (b_FullScreenHotKey && (!(GetAsyncKeyState(VK_MENU) & 0x8000) || !(GetAsyncKeyState(VK_RETURN) & 0x8000)))
		b_FullScreenHotKey = false;
	else if (!b_FullScreenHotKey && (GetAsyncKeyState(VK_MENU) & 0x8000) && (GetAsyncKeyState(VK_RETURN) & 0x8000))
	{
		bitFlags ^= FULLSCREEN;
		b_FullScreenHotKey = true;
		FullScreenWindow(hWnd);
	}

	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
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