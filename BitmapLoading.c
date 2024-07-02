/*
Happy Birthday Win32
Menu Based Bitmap Loading Program

*/


#include <windows.h>
#include "Window.h"


// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK AboutDialogProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE ghInstance;
int flag;


// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	HMENU hMenu;
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("Happy Birthday");
	HWND hwnd;
	MSG msg;

	ghInstance  = hInstance;
	hMenu		= LoadMenu(hInstance, MAKEINTRESOURCE(MYMENU));

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

	
	// Create the window
	// 11 parameters
	hwnd = CreateWindow(szClassName,
		TEXT("Happy 30th Birthday Win32!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		hMenu,
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
	HDC 		hdc  = NULL;	// handle to device
	PAINTSTRUCT ps;				// paint information struct
	RECT 		rc;			// rectangle	
	static 		HBITMAP hBitmap;
	HDC 		hdcMem;
	BITMAP		bitmap;
	static 		HINSTANCE hInstance;
	
	//TCHAR str[] = TEXT("Hello, Yashraj Gaikwad!");

	//code
	switch (iMsg)
	{
		case WM_CREATE:
		PlaySound(TEXT("Interstellar.wav"), NULL, SND_FILENAME | SND_ASYNC);
		hInstance = GetModuleHandle(NULL);
		
		break;
		
		case WM_COMMAND:
		
			switch(LOWORD(wParam))
			{
				case IDM_MICROSOFT:
					flag = 1;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;
				
				case IDM_SIR:
					flag = 2;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;
				
				case IDM_MADAM:
					flag = 3;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;
				
				
				case IDM_WIN32:
					flag = 4;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;
				
				case IDM_DAVID_CUTLER:
					flag = 5;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;

				case IDM_JEFFREY_RICHTER:
					flag = 6;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;

				case IDM_MARK_RUSSINOVICH:
					flag = 7;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;

				case IDM_PETER_VISCAROLLA:
					flag = 8;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;

				case IDM_WALTER_ONEY:
					flag = 9;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;

				case IDM_CHARLES_PETZOLD:
					flag = 10;
					InvalidateRect(hwnd, NULL, TRUE);
				
				break;


				case IDM_APPABOUT:
					DialogBox(ghInstance, TEXT("about"), hwnd, AboutDialogProc);
				break;
					
			}
		
		break;
		
		
		case WM_PAINT:
			// Step 1: Get the painter	
			hdc = BeginPaint(hwnd, &ps);	// hdc - handle to device context
			
			// Step 2: Get the client area rectangle
			GetClientRect(hwnd, &rc);
			
			hdcMem = CreateCompatibleDC(NULL);
			
			SetWindowPos(hwnd, NULL, 0, 0, GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), SWP_FRAMECHANGED);

			if(flag == 1)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MICROSOFT));
			
			if(flag == 2)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(SIR));
			
			if(flag == 3)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MADAM));
			
			
			if(flag == 4)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(WIN32));
					
			

			if(flag == 5)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(DAVID_CUTLER));
					
			

			if(flag == 6)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(JEFFREY_RICHTER));
					
			

			if(flag == 7)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(MARK_RUSSINOVICH));
					
			

			if(flag == 8)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(PETER_VISCAROLLA));
					
			

			if(flag == 9)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(WALTER_ONEY));
					
			

			if(flag == 10)
				hBitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(CHARLES_PETZOLD));
					
			
			SelectObject(hdcMem, hBitmap);
			GetObject(hBitmap, sizeof(BITMAP), &bitmap);
			StretchBlt(hdc, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
			
			if(hBitmap)
			{
				DeleteObject(hBitmap);
				hBitmap = NULL;
			}
			
			if(hdcMem)
			{
				DeleteDC(hdcMem);
				hdcMem = NULL;
			}
			
			if(hdc)
			{
				// Step 6: Release Painter
				EndPaint(hwnd, &ps);
				hdc = NULL;
			}
			
			break;
		
		case WM_DESTROY:
			MessageBox(hwnd, TEXT("Thank you!"), TEXT("Message: "), MB_OK);
			PostQuitMessage(0);
		break;

			default:
			break;
	}
	
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

BOOL CALLBACK AboutDialogProc(HWND hdlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch(iMsg)
	{
		case WM_INITDIALOG:
			return TRUE;
		
		case WM_COMMAND:
		
			switch(LOWORD(wParam))
			{
				
				case IDOK:
				case IDCANCEL:
					EndDialog(hdlg, FALSE);
					return TRUE;
			}
			
		break;
	}
	
	return FALSE;
}




