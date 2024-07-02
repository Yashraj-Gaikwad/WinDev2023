/*
Hellow World program

painting and repainting
WM_PAINT code
5-6 lines extra

*/



#include <windows.h>
#include "Window.h"


// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/* callback function - we cant decide return type, it is os dependent
 1) return type is os specified 
 2) you cant decide the no.,types, order of parameters, it os specified
 3) you cant decide the calling convention, it is os specific

*/

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("MyFirstWindow");
	HWND hwnd;
	MSG msg;


	// code
	// 12 members
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	
	// change
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));				// IDI - id of default icon
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));

	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;


	// Register window class
	RegisterClassEx(&wndclass);		// returns atomic string

	// error checking
	/*
		if(!RegisterClassEx(&wndclass))
		{
			MessageBox(NULL,TEXT("RegisterClass Failed"),MB_OK | MB_ICONERROR);
			
			exit(-1);
		}
	*/

	// Create the window
	// 11 parameters
	hwnd = CreateWindow(szClassName,
		TEXT("Yashraj Gaikwad: First Window"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);		// LPCREATESTRUCT - long pointer to createstruct
	
	// error checking
	/*
		if(hwnd == NULL)
		{
			MessageBox(NULL,TEXT("Creat Window Failed"),MB_OK | MB_ICONERROR);
			
			exit(-1);
		}
	*/

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
	HDC 		hdc  = NULL;	// handle to device
	PAINTSTRUCT ps;		// paint information struct
	RECT 		rect;	// rectangle	
	
	TCHAR str[] = TEXT("Hello, Yashraj Gaikwad!");

	//code
	switch (iMsg)
	{
		case WM_CREATE:
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
		
		case WM_PAINT:
			// Step 1: Get the painter	
			hdc = BeginPaint(hwnd, &ps);	// hdc - handle to device context
			
			// Step 2: Get the client area rectangle
			GetClientRect(hwnd, &rect);
			
			hFont = CreateFont(0,closest_match, escapement, orientation, FW_DONTCARE,
							   no_italic, no_ul, no_xout, ANSI_CHARSET, OUT_TT_PRECIS,
							   CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, VARIABLE_PITCH, TEXT("Hello"));

			hOldFont = (HFONT) SelectObject(hdc, hFont);


			// Step 3: Set Text Background Color to black
			SetBkColor(hdc, RGB(0,0,0));

			// Step 4: Set the text color to green
			SetTextColor(hdc, RGB(255,0,0));
			
			// Step 5: Draw text
			DrawText(hdc,									// painter
					str,									// string
					-1,										// when you want entire string to drawn, can also say - wcslen
					&rect,									// client rectangle address
					DT_SINGLELINE | DT_CENTER | DT_VCENTER);// DT - Draw Text
			
			// DT_CENTER - horizontal centre
			// DT_VCENTER - vertical centre
			
			SelectObject(hdc, OldFont);
			
			DeleteObject(hFont);
			
			
			if(hdc)
			{
				// Step 6: Release Painter
				EndPaint(hwnd, &ps);
				hdc = NULL;
			}
			
			break;
		
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

			default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



