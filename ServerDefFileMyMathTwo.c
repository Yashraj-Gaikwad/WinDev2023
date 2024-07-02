/*
Function exported using defFile method

After change in this program

1) rebuild dll

2) replace dll, lib, h in client

*/

#include <windows.h>

// entry point function
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved)
{
	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		
		break;
		
		case DLL_THREAD_ATTACH:
		
		break;
		
		case DLL_THREAD_DETACH:
		
		break;
		
		case DLL_PROCESS_DETACH:
		
		break;
		
	}
		
	return TRUE;
	
}

// MakeSquare will be exported by the dll
int MakeCube(int num)
{
	// function prototype
	void CheckNum(int num);
	
	// Code
	CheckNum(num);
	
	return (num * num * num);
}

// CheckNum is for internal usage for the dll
void CheckNum(int num)
{
	if(num < 0)
		MessageBox(NULL, TEXT("Number is negative"), TEXT("Message:"), MB_ICONINFORMATION | MB_OK);
	
}











