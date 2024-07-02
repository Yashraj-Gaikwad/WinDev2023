/*
Multi-threading program

_beginthread(Thread, 0, NULL);

_endthread();

*/



#include <windows.h>
#include "Window.h"
#include <process.h>

// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Thread Declarations
DWORD WINAPI ThreadProcOne(LPVOID);
DWORD WINAPI ThreadProcTwo(LPVOID);

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("Multi-threading Program");
	static HWND hwnd;	// for ThreadProc
	MSG msg;


	// code
	// 12 members
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	
	// change
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));				// IDI - id of default icon
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));

	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;


	// Register window class
	RegisterClassEx(&wndclass);		// returns atomic string


	// Create the window
	// 11 parameters
	hwnd = CreateWindow(szClassName,
		TEXT("Multi-threading Program"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);		// LPCREATESTRUCT - long pointer to createstruct
	
	
	// Show the window
	ShowWindow(hwnd, iCmdShow);

	// Update the window
	UpdateWindow(hwnd);

	// Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

}




// callback window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// Variable Declarations
	static HANDLE hThread1 = NULL;
	static HANDLE hThread2 = NULL;
	
	//code
	switch (iMsg)
	{
		case WM_CREATE:
		

		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);

		hThread1 = CreateThread(NULL,									// LPSECURITY_ATTRIBUTES
								0,										// Thread Stack Size
								(LPTHREAD_START_ROUTINE)ThreadProcOne,	// TypeCast Thread Fumction name
								(LPVOID)hwnd,							// Parameter to ThreadProcOne
								0,										// Flags
								NULL);									// ThreadID
									

		hThread2 = CreateThread(NULL,									// LPSECURITY_ATTRIBUTES
								0,										// Default Thread Stack Size
								(LPTHREAD_START_ROUTINE)ThreadProcTwo,	// TypeCast Thread Fumction name
								(LPVOID)hwnd,							// Parameter to ThreadProcOne
								0,										// Flags
								NULL);									// ThreadID
			
		break;
		
		case WM_LBUTTONDOWN:
			MessageBox(NULL, TEXT("Hello, Multi-Threading"), TEXT("HelloMT"), MB_YESNO | MB_ICONEXCLAMATION);
		break;


		case WM_DESTROY:
			
			if(hThread2)
				CloseHandle(hThread2);

			if(hThread1)
				CloseHandle(hThread1);

			PostQuitMessage(0);
		break;

		default:
		
		break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

// Defining ThreadProcOne
DWORD WINAPI ThreadProcOne(LPVOID param)
{
	// Variable Declarations
	HDC 			hdc;
	unsigned long 	i;
	TCHAR 			str[256];

	// Code
	hdc = GetDC((HWND)param);	// type casting
	
	SetWindowPos((HWND)param, NULL, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SWP_FRAMECHANGED);

	SetBkColor(hdc, RGB(0, 0, 0));	// black

	SetTextColor(hdc, RGB(0, 255, 0));	// blue

	// incrementing lop
	for(i = 0; i <= LONG_MAX; i++)
	{
		wsprintf(str, TEXT("Incrementing: %d"), i);

		TextOut(hdc, 5, 5, str, wcslen(str));	// wcslen - wide character string length
	}

	if(hdc)
	{
		ReleaseDC((HWND)param, hdc);
		hdc = NULL;
	}

	
	return 0;
}

// Defining ThreadProcTwo
DWORD WINAPI ThreadProcTwo(LPVOID param)
{
	// Variable Declarations
	HDC 			hdc;
	unsigned long  	l;
	TCHAR 			str[256];

	// Code
	hdc = GetDC((HWND)param);	// type casting

	SetBkColor(hdc, RGB(0, 0, 0));	// black

	SetTextColor(hdc, RGB(255, 0, 0));	// blue

	// decrementing loop
	// if negative values use recursive
	// greater than sign not compatible with loop
	for(l = LONG_MAX; l != -1; l--)
	{
		wsprintf(str, TEXT("Decrementing: %d"), l);

		TextOut(hdc, 5, 20, str, wcslen(str));
	}


	if(hdc)
	{
		ReleaseDC((HWND)param, hdc);
		hdc = NULL;

	}


	return 0;
}


