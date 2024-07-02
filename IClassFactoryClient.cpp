/*
Client Of Class Factory Dll Server With RegFile

*/

#define UNICODE 	// for win32 functions
#include <windows.h>
#include "Window.h"
#include "AutomationServer.h"
#include <stdio.h>				// For us, to use swprintf_s()

// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
// Interface Pointers
// Virtual Table Created in memory

IMyMath	*pIMyMath	= NULL;

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX 	wndclass;
	TCHAR 		szClassName[] = TEXT("ComClient");
	HWND 		hwnd;
	MSG 		msg;
	HRESULT 	hr = S_OK;		// Default value

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
		TEXT("Client Of Automation DLL"),
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
	HRESULT hr = S_OK;
	int 	iNum1 = 40;
	int 	iNum2 = 60;
	int 	iSub, iSum;
	TCHAR 	str[256];



	// function declarations
	void SafeInterfaceRelease(void);
	void ComErrorDescriptionString(HWND, HRESULT);

	//code
	switch (iMsg)
	{
		case WM_CREATE:
		// Refer to CoCreateInstance pseudo code 

		// COM loads Library
		hr = CoCreateInstance(CLSID_MyMath,			// GUID of class MyMath
			 NULL, 									// NULL for no aggregation
			 CLSCTX_INPROC_SERVER,					// Class Context in process server (.exe)
			 IID_IMyMath,								// IID of IMyMath
			 (void **)&pIMyMath);						// pointer to store interface pointer						
			// virtual table to implementation table
			
		if(FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("IMyMath Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// call SumOfTwoIntegers() of IMyMath
		pIMyMath -> SumOfTwoIntegers(iNum1, iNum2, &iSum);


		// display the result
		wsprintf(str, TEXT("Sum of %d and %d = %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);


		// again reinitialize
		iNum1 = 30;
		iNum2 = 50;

		// call SubtractionOfTwoIntegers()
		pIMyMath -> SubtractionOfTwoIntegers(iNum1, iNum2, &iSub);

		// display result
		wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iSub);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);


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

// Naked C style Function
// User Defined Function
void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
	// variable declarations
	WCHAR* szErrorMessage = NULL;
	WCHAR str[255];

	if(FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);
	
	if(FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&szErrorMessage, 0, NULL) != 0)
	{
		// FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM 	= System Pl. allocate a buffer for my error string
		// NULL														   	= No hwnd
		// hr 															= I want error string for this hr		  	
		// MAKELANGID() or en-US										= Primary Lang ID, Secondary Lang ID
		// (LPSTR)&szErrorMessage										= Returns Error Message
		// 0															= Not passing any array
		// NULL															= If passing array, the which members


		// Secure wprintf()
		// To print value of pointer use secure function
		// s 		= print in string
		// wprintf 	= wide char UNICODE string printf
		// _s 		= secure
		// %#		= HexaDecimal Value
		swprintf_s(str, TEXT("%08X : %s"), hr, szErrorMessage );

		// Thread Local Storage
		LocalFree(szErrorMessage);
	}

	else
		swprintf_s(str, TEXT("[Could not find description for error # %#x.] \n"), hr);


	MessageBox(hwnd, str, TEXT("COM Error"), MB_OK);


}



// safety measure to release interfaces
void SafeInterfaceRelease(void)
{
	if(pIMyMath)
	{
		pIMyMath -> Release();
		pIMyMath = NULL;
		
	}

}

