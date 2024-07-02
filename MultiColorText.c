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
	static int 	iPaintFlag = 0;
	//HBRUSH      hBrush     = NULL;

	TCHAR str[] = TEXT("Hello, Yashraj Gaikwad!");

	//code
	switch (iMsg)
	{
		case WM_CREATE:
		PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
		break;
		
		case WM_PAINT:
			
			hdc = BeginPaint(hwnd, &ps);	// hdc - handle to device context
			
			
			GetClientRect(hwnd, &rect);
			
			if(iPaintFlag == 1)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(255, 0, 0));		// red
			}

			else if(iPaintFlag == 2)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(0, 255, 0));		// green
			}

			else if(iPaintFlag == 3)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(0, 0, 255));		// blue
			}

			else if(iPaintFlag == 4)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(0, 255, 255));	// cyan
			}

			else if(iPaintFlag == 5)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(255, 0, 255));	// magenta
			}

			else if(iPaintFlag == 6)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(255, 255, 0));		// yellow
			}

			else if(iPaintFlag == 7)
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(255, 255, 255)); 		// white
			}

			else if(iPaintFlag == 8)
			{
				SetBkColor(hdc, RGB(255, 255, 255));
				SetTextColor(hdc, RGB(0, 0, 0));		// black
			}

			else
			{
				SetBkColor(hdc, RGB(0,0,0));
				SetTextColor(hdc, RGB(128, 128, 128));		// grey
			}
			
			
			DrawText(hdc,									// painter
					str,									// string
					-1,										// when you want entire string to drawn, can also say - wcslen
					&rect,									// client rectangle address
					DT_SINGLELINE | DT_CENTER | DT_VCENTER);// DT - Draw Text
			
			// give the brush to painter
			//SelectObject(hdc, hBrush);

			// color the window
			//FillRect(hdc, &rect, hBrush);
			

			// release the brush
			//if(hBrush)
			//{
			//	DeleteObject(hBrush);
			//	hBrush = NULL;
			//}

			
			if(hdc)
			{
				// Step 6: Release Painter
				EndPaint(hwnd, &ps);
				hdc = NULL;
			}
			
			break;
		
		
		case WM_CHAR:
			iPaintFlag = 0;		// default grey color window
		
			switch(LOWORD(wParam))
			{
				case 'R':
				case 'r':
					
					iPaintFlag = 1;		// red
					InvalidateRect(hwnd, NULL, TRUE); 	// (hwnd, entire window, repaint) calls WM_PAINT

					break;

				case 'Y':
				case 'y':
					iPaintFlag = 6;		// yellow
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

				case 'G':
				case 'g':
					iPaintFlag = 2;		// green
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

				case 'B':
				case 'b':
					iPaintFlag = 3;		// blue
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

				case 'K':
				case 'k':
					iPaintFlag = 8;		// black
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

	
				case 'C':
				case 'c':
					iPaintFlag = 4;		// cyan
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;


				case 'M':
				case 'm':
					iPaintFlag = 5;		// magenta
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

				case 'W':
				case 'w':
					iPaintFlag = 7;		// white
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

				default:
					iPaintFlag = 0;		// grey
					InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

					break;

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



