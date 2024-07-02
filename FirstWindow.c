/*
Shows First Window

*/

// Main win32 header file
#include <windows.h>

// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// typedef - long calling_convention Function_name(void*, unsigned int, unsigned int, long)

/* callback function - we cant decide return type, it is os dependent
 1) return type is os specified 
 2) you cant decide the no.,types, order of parameters, it os specified
 3) you cant decide the calling convention, it is os specific

*/

// Entry point function
// typedef - int __stdcall function_name(void*, void*, char**, int)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;	// struct, window class extended
	TCHAR szClassName[] = TEXT("MyFirstWindow");
	HWND hwnd;
	MSG msg;	// struct 


	// code
	ZeroMemory((void*)&wndclass, sizeof(WNDCLASSEX));	// Initializes struct to NULL
	
	// 12 members
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// macro
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(DKGREY_BRUSH);  // macro, GDI function
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);  // macro
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);  // macro
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  // macro
	wndclass.lpfnWndProc = WndProc;		// Register your windows callback function
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
		WS_OVERLAPPEDWINDOW,  // macro
		CW_USEDEFAULT,   // macro
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
	//code
	switch (iMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



