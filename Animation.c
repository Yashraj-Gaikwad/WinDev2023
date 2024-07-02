/*
Animation

*/



#include <windows.h>
#include "Window.h"
#define YAG_TIMER 117
#define CIRCLE_DIAMTER 100


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
	UpdateWindow(hwnd);		// Sents WM_PAINT

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
	HDC 		hdc        = NULL;	// handle to device
	PAINTSTRUCT ps;			 		// paint information struct
	static  	RECT 		rect;				// rectangle	
	HBRUSH      hBrush     = NULL;
	
	static int y 	= 0;
	static int x1 	= -(CIRCLE_DIAMTER * 0);
	static int x2	= -(CIRCLE_DIAMTER * 1);
	static int x3 	= -(CIRCLE_DIAMTER * 2);
	static int x4 	= -(CIRCLE_DIAMTER * 3);
	static int x5 	= -(CIRCLE_DIAMTER * 4);
	static int x6 	= -(CIRCLE_DIAMTER * 5);
	static int x7 	= -(CIRCLE_DIAMTER * 6);
	static int x8 	= -(CIRCLE_DIAMTER * 7);
	static int x9 	= -(CIRCLE_DIAMTER * 8);
	static int x10 	= -(CIRCLE_DIAMTER * 9);

	static BOOL bReset = TRUE;

	//code
	switch (iMsg)
	{
		case WM_CREATE:
			PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);
			SetTimer(hwnd, YAG_TIMER, 5, NULL);	// null - timerproc
		break;
		
		
		case WM_PAINT:
			
			hdc = BeginPaint(hwnd, &ps);	// hdc - handle to device context
			
			GetClientRect(hwnd, &rect);
		
			if(bReset == TRUE)
			{
				y = (rect.bottom - rect.top) / 2;
			}

			hBrush = CreateSolidBrush(RGB(255, 0, 0));		// red

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x1, y, x1 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}

			
			hBrush = CreateSolidBrush(RGB(0, 255, 0));		// green

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x2, y, x2 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(0, 0, 255));		// blue

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x3, y, x3 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(0, 255, 255));		// cyan

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x4, y, x4 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(255, 0, 255));		// magenta

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x5, y, x5 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(255, 255, 0));		// yellow

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x6, y, x6 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(255, 255, 255));		// white

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x7, y, x7 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(128, 128, 128));		// grey

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x8, y, x8 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(255, 128, 0));		// orange

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x9, y, x9 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}


			
			hBrush = CreateSolidBrush(RGB(128, 128, 255));		// lavender

			// give the brush to painter
			SelectObject(hdc, hBrush);

			
			Ellipse(hdc, x10, y, x10 + CIRCLE_DIAMTER, y + CIRCLE_DIAMTER);		//(hdc, left, top, right, bottom) 
			
			// release the brush
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}



			if(hdc)
			{
				
				EndPaint(hwnd, &ps);
				hdc = NULL;
			}
			
		break;
		
		
		case WM_TIMER:
			KillTimer(hwnd, YAG_TIMER);
			x1++;
			x2++;
			x3++;
			x4++;
			x5++;
			x6++;
			x7++;
			x8++;
			x9++;
			x10++;

			if(x10 > rect.right)
			{
				// resetting x coordinates

				x1 	= -(CIRCLE_DIAMTER * 0);
				x2	= -(CIRCLE_DIAMTER * 1);
				x3 	= -(CIRCLE_DIAMTER * 2);
				x4 	= -(CIRCLE_DIAMTER * 3);
				x5 	= -(CIRCLE_DIAMTER * 4);
				x6 	= -(CIRCLE_DIAMTER * 5);
				x7 	= -(CIRCLE_DIAMTER * 6);
				x8 	= -(CIRCLE_DIAMTER * 7);
				x9 	= -(CIRCLE_DIAMTER * 8);
				x10 = -(CIRCLE_DIAMTER * 9);

			}

			

			InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

			SetTimer(hwnd, YAG_TIMER, 5, NULL);

		break;
		
		case WM_KEYDOWN:
		
		switch(LOWORD(wParam))
		{
			case VK_ESCAPE:
				DestroyWindow(hwnd);	// closes window

				break;

			case VK_UP:
				// decrement y
				bReset = FALSE;
				y -= 50;

				if(y < rect.top)
				{
					y = rect.top;
				}
				
				InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

				break;

			case VK_DOWN:
				// increment y
				bReset = FALSE;
				y += 50;

				if( y  > (rect.bottom - CIRCLE_DIAMTER))
				{
					y = rect.bottom - CIRCLE_DIAMTER;
				}

				InvalidateRect(hwnd, NULL, TRUE); 	// calls WM_PAINT

				break;


			default:

				break;
		}

		break;

		case WM_CHAR:
			
			switch(LOWORD(wParam))
			{
				case 'R':
				case 'r':
					bReset = TRUE;
	
				break;
				
				default:
				
				break;

			}


		break;

		case WM_LBUTTONDOWN:

			bReset = FALSE;
			// xcoordinate
			x1 = LOWORD(lParam);
			x2 = x1 - (CIRCLE_DIAMTER * 1);
			x3 = x1 - (CIRCLE_DIAMTER * 2);   
			x4 = x1 - (CIRCLE_DIAMTER * 3);
			x5 = x1 - (CIRCLE_DIAMTER * 4);
			x6 = x1 - (CIRCLE_DIAMTER * 5);
			x7 = x1 - (CIRCLE_DIAMTER * 6);
			x8 = x1 - (CIRCLE_DIAMTER * 7);
			x9 = x1 - (CIRCLE_DIAMTER * 8);
			x10 = x1 - (CIRCLE_DIAMTER * 9);

	
			// ycoordinate
			y = HIWORD(lParam);

		break;



		case WM_DESTROY:
			
			PostQuitMessage(0);
		break;

			default:

			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



