/*
Client program

*/


#include <windows.h>
#include "Window.h"


// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


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
	HMODULE hDll = NULL;
	int num = -5;
	int cube;
	TCHAR str[256];

	// Step 2 A):
	// Define function pointer
	typedef int (*MakeCubeFnPtr)(int); 	// function pointer type name

	// Declare function pointer variable
	MakeCubeFnPtr ptrFuncMakeCube = NULL;


	//code
	switch (iMsg)
	{
		case WM_CREATE:
		
		// Step 1: Load The Library
		hDll = LoadLibrary(TEXT("MyMathTwo.dll"));

		// error checking
		if(hDll == NULL)
		{
			MessageBox(hwnd, TEXT("Load Library Failed"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}
		
		// Step 2: Call function from dll
		// Step 2 A) : Declare function pointer to store address of function (see above in variable declarations)

		// Step 2) B): Get address of the dll function from all and store in function pointer
		ptrFuncMakeCube = (MakeCubeFnPtr) GetProcAddress(hDll, "MakeCube");		// type casting
		// finds the dll

		if(ptrFuncMakeCube == NULL)
		{
			MessageBox(hwnd, TEXT("MAke Cube Function Failed"), TEXT("Error"), MB_OK);
			FreeLibrary(hDll);
			hDll = NULL;
			DestroyWindow(hwnd);
		}

		// function calling
		cube = ptrFuncMakeCube(num);

		wsprintf(str, TEXT("Cube of %d is %d"), num, cube);

		MessageBox(hwnd, str, TEXT("Cube"), MB_OK);

		if(hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
	
		DestroyWindow(hwnd);

		break;


		case WM_DESTROY:

			PostQuitMessage(0);
		break;

		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}



