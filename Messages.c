/*
Shows messages on click

*/


#include <windows.h>
#include <tchar.h>

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
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;


	// Register window class
	RegisterClassEx(&wndclass);

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
		NULL);

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
	// variable declarations
	wchar_t STR(100);
	wchar_t ch(100);
	int x,y;

	//code
	switch (iMsg)
	{
		case WM_CREATE:		// lparam
			//wsprintf(STR,"Window Created");
			MessageBox(NULL, TEXT("Messages"),szClassName,MB_OK | MB_ICONINFORMATION);

			break;

		case WM_KEYDOWN:	// Complex
			
			MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
			
			switch(wParam)	// to know which key is pressed
			{
				case VK_SPACE:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;
				
				case VK_ESCAPE:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;
				
				case VK_ADD:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;

				case VK_SUBTRACT:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;

				case VK_UP:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;
				
				case VK_DOWN:		// VK - Virtual Keyboard
					MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);
					break;
				
			}


			break;
		
		case WM_CHAR:	// Simple
			
			ch = wParam;
			wsprintf(ch, "WM_CHAR Message Received : : %c");
			MessageBox(NULL, TEXT("Message Received"),szClassName,MB_OK | MB_ICONINFORMATION);

			break;
		
		case WM_LBUTTONDOWN;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			//wsprintf(STR,TEXT("Left mouse button click at : (%d, %d)"), x,y);
			MessageBox(NULL, TEXT("Messages"),szClassName,MB_OK | MB_ICONINFORMATION);
			break;
		
		case WM_RBUTTONDOWN;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			//wsprintf(STR,TEXT("Right mouse button click at : (%d, %d)"), x,y);
			MessageBox(NULL, TEXT("Messages"),szClassName,MB_OK | MB_ICONINFORMATION);
			break;



		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



