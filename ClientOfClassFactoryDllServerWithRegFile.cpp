#define UNICODE		// why unicode
#include <windows.h>
#include <"ClassFactoryDllServerWithRegFile.h">

// global function declarations
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declarations
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;

// WinMain
int WIN




HRESULT CSumSubtractClassFactory::CreateInstance()
{

;
HRESULT hr = S_OK;

// object
class pCSumSubtract = new CSumSubtract; 	// Call to constructor

if(pCSumSubtract == NULL)
	return (E_OUTOFMEMORY);

// object has memory
// Co-class Implemented Query Interface 
hr = pCSumSubtract -> QueryInterface(riid, ppv);

// Implemented Release
pCSumSubtract -> Release();

// why calling release? 

return();


}

HRESULT	::LockServer() 	// boolean flags
{
	// what is lock server?

	if(fLock)	// f-Lock
		
	else

	return();
}

// C like functions
// Procedure functions
// Naked Functions
// extern C for exported function, to avoid name mangaling


extern C HRESULT __stdcall DllGetClassObject()
{
	// REFCLSID - Reference to class id
	
	// CLSID given in header file
	if()
	
	// create class factory object
	// call to constructor



}

extern C HRESULT __stdcall DLlCanUnloadNow()
{
	// only if both are true
	if()

	else


}

