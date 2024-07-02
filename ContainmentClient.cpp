/*
Client Of Containment Dll Server With RegFile

*/

#define UNICODE 	// for win32 functions
#include <windows.h>
#include "Window.h"
#include "HeaderForClientOfComponentWithRegFile.h"

// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
// Interface Pointers
// Virtual Table Created in memory

ISum 	  		*pISum 	  			= NULL;		// pointer to ISum
ISubtract 		*pISubtract 		= NULL;		// pointer to ISubtract
IMultiplication *pIMultiplication 	= NULL;
IDivision       *pIDivision 		= NULL;

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	TCHAR szClassName[] = TEXT("ComClient");
	HWND hwnd;
	MSG msg;
	HRESULT hr = S_OK;		// Default value

	// COM Initialization
	hr = CoInitialize(NULL);	// LPVOID Reserved for internal use

	// Error Handling
	if(FAILED(hr))	// COM Macro - FAILED(hr), SUCCEEDED(hr)
	{
		MessageBox(NULL, TEXT("COM Library Can Not Be Initialized. \n Program will now exit"), TEXT("COM Error"), MB_OK);
		TEXT("Program Error"), MB_OK;
		
		exit(0);
	}



	// code
	// 12 members
	wndclass.cbSize 			= sizeof(WNDCLASSEX);
	wndclass.style 				= CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra 		= 0;
	wndclass.cbWndExtra 		= 0;
	wndclass.hInstance 			= hInstance;
	wndclass.hbrBackground		= (HBRUSH)GetStockObject(BLACK_BRUSH);
	
	// change
	wndclass.hIcon 				= LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));	// IDI - id of default icon
	wndclass.hIconSm 			= LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));

	wndclass.hCursor 			= LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc		= WndProc;
	wndclass.lpszClassName 		= szClassName;
	wndclass.lpszMenuName 		= NULL;


	// Register window class
	RegisterClassEx(&wndclass);		// returns atomic string

	

	// Create the window
	// Window created in memory
	// WM_CREATE sent
	// 11 parameters
	hwnd = CreateWindow(szClassName,
		TEXT("Client Of Com Dll Server"),
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

	// COM Un-initialization
	CoUninitialize(); 	// CoFreeUnused Library
	
	return (int)msg.wParam;

}

// callback window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HRESULT hr;
	int iNum1 = 40;
	int iNum2 = 60;
	int iSum, iSub, iMul, iDiv;
	TCHAR str[256];



	// function declarations
	void SafeInterfaceRelease(void);


	//code
	switch (iMsg)
	{
		case WM_CREATE:
		// Refer to CoCreateInstance pseudo code 

		// COM loads Library
		hr = CoCreateInstance(CLSID_SumSubtract,	// GUID of class SumSubtract
			 NULL, 									// NULL for no aggregation
			 CLSCTX_INPROC_SERVER,					// Class Context in process server (.exe)
			 IID_ISum,								// IID of ISum
			 (void **)&pISum);						// pointer to store interface pointer						
			// virtual table to implementation table
			
		if(FAILED(hr))
		{
			MessageBox(hwnd, TEXT("ISum Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
			
			DestroyWindow(hwnd);
		}

		// call SumOfTwoIntegers() of ISum
		pISum -> SumOfTwoIntegers(iNum1, iNum2, &iSum);

		// display the result
		wsprintf(str, TEXT("Sum of %d and %d = %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// call QueryInterface() on Isum to get Isubtract's pointer
		hr = pISum -> QueryInterface(IID_ISubtract, (void **)&pISubtract);

		if(FAILED(hr))
		{
			MessageBox(hwnd, TEXT("ISubtract Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
			
			DestroyWindow(hwnd);
		}
		
		// If passed


		// again reinitialize
		iNum1 = 40;
		iNum2 = 60;

		// again call SumOfTwoIntegers()
		pISubtract -> SubtractionOfTwoIntegers(iNum1, iNum2, &iSub);


		// display result
		wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iSub);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// Call QueryInterface() On ISubtract to get IMultiplication
		pISubtract -> QueryInterface(IID_IMultiplication, (void **)&pIMultiplication);
		
		if(FAILED(hr))
		{
			MessageBox(hwnd, TEXT("IMultiplication Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
			
			DestroyWindow(hwnd);
		}
		
		
		// re-initialze
		iNum1 = 40;
		iNum2 = 60;
		
		// call MultiplicationOfTwoIntegers()
		pIMultiplication -> MultiplicationOfTwoIntegers(iNum1, iNum2, &iMul);
		
		// display
		wsprintf(str, TEXT("Multiplication of %d and %d = %d"), iNum1, iNum2, iMul);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// Call QueryInterface() On IMultiplication to get IDivision
		pIMultiplication -> QueryInterface(IID_IDivision, (void **)&pIDivision);
		
		if(FAILED(hr))
		{
			MessageBox(hwnd, TEXT("IDivision Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
			
			DestroyWindow(hwnd);
		}
		
		
		// re-initialze
		iNum1 = 40;
		iNum2 = 60;
		
		// call MultiplicationOfTwoIntegers()
		pIDivision -> DivisionOfTwoIntegers(iNum1, iNum2, &iDiv);
		
		// display
		wsprintf(str, TEXT("Division of %d and %d = %d"), iNum1, iNum2, iDiv);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);


		// exit the application
		DestroyWindow(hwnd);


		break;


		case WM_DESTROY:
			
			// relinquish the interfaces
			SafeInterfaceRelease();

			PostQuitMessage(0);
		break;

		default:
			break;
	}

	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}

// safety measure to release interfaces
void SafeInterfaceRelease(void)
{
	if(pISubtract)
	{
		pISubtract -> Release();
		pISubtract = NULL;
	}

	if(pISum)
	{
		pISum -> Release();
		pISum = NULL;

	}

	if(pIMultiplication)
	{
		pIMultiplication -> Release();
		pIMultiplication = NULL;
	}

	if(pIDivision)
	{
		pIDivision -> Release();
		pIDivision = NULL;
		
	}

}

