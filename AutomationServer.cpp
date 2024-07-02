#define UNICODE
#include <windows.h>
#include <stdio.h>				// For us, to use swprintf_s()
#include "AutomationServer.h"

// class declarations
class CMyMath : public IMyMath
{
private:
	long m_cRef;					// for reference counting
	ITypeInfo *m_pITypeInfo;		// Interface Type Information

public:
	// constructor
	CMyMath(void);
	
	// destructor
	~CMyMath(void);
	
	// IUnknown specific method declarations
	// IUnknown comes from IDispatch
	HRESULT __stdcall QueryInterface (REFIID, void **);	// REFIID = Reference of Interface Identifers, void** = double indirection
	ULONG   __stdcall AddRef		 (void);
	ULONG	__stdcall Release		 (void);
	
	// IDispatch Methods
	HRESULT __stdcall GetTypeInfoCount	(UINT*);
	HRESULT __stdcall GetTypeInfo		(UINT, LCID, ITypeInfo**);
	HRESULT __stdcall GetIDsOfNames 	(REFIID, LPOLESTR*, UINT, LCID, DISPID*);	// LP-OLE-STR, DISP-ID-*
	HRESULT __stdcall Invoke			(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*); // IMP FUNCTION DISP-PARAMS-*

	// IMyMath methods
	HRESULT __stdcall SumOfTwoIntegers			(int, int, int *);
	HRESULT __stdcall SubtractionOfTwoIntegers  (int, int, int *);
	
	// custom method
	HRESULT InitInstance(void);	// IMP FUNCTION
};

class CMyMathClassFactory :public IClassFactory
{
private:
	long m_cRef;

public:

	// constructor
	CMyMathClassFactory(void);
	
	//destructor
	~CMyMathClassFactory(void);
	
	// IUnknown methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG	__stdcall AddRef		(void);
	ULONG	__stdcall Release		(void);
	
	// IClassFactory methods
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer	(BOOL);
	
};

// global variable declarations
long glNumberOfActiveComponents = 0;	// no. of active components
long glNumberOfServerLocks = 0;			// no. of locks on this dll

// GUID LIBID - Type Library ID, IMP
// For Internal Server Use, Client Should Not Know - IDL uses
// {62B0F15F-EF65-48E7-8938-35937C70A295} 
const GUID LIBID_AutomationServer = {0x62b0f15f, 0xef65, 0x48e7, 0x89, 0x38, 0x35, 0x93, 0x7c, 0x70, 0xa2, 0x95};

// DllMain
// runs when dll gets loaded after CoCreateInstance
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	switch(dwReason)
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
	
	return(TRUE);
}


// Implementation of CMyMath Constructor
CMyMath::CMyMath(void)
{
	// hardcoded initialization
	m_cRef = 1;
	m_pITypeInfo = NULL;	// IMP

	// Thread safe atomic operation
	InterlockedIncrement(&glNumberOfActiveComponents);	// increment global counter
}

// Implementation CMyMath Destructor
CMyMath::~CMyMath(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);	// decrement global counter
	
}

// Implementation of CMyMath IUnknown
HRESULT CMyMath::QueryInterface(REFIID riid, void **ppv)
{
	// runtime type identification
	// write if-else-if block separate as COM Discipline
	if(riid == IID_IUnknown)
		*ppv = static_cast<IMyMath *>(this);	// compile time
	
	else if(riid == IID_IDispatch)				// we want this
		*ppv = static_cast<IMyMath *>(this);
	
	else if(riid == IID_IMyMath)				// we want this
		*ppv = static_cast<IMyMath *>(this);
	
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv) -> AddRef();	// whoever you are call the addref inside your iunknown

	return(S_OK);
	
}

ULONG CMyMath::AddRef(void)
{
	InterlockedIncrement(&m_cRef);	// 
	return(m_cRef);
}

ULONG CMyMath::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1 later 0 after pIMyMath
	
	if(m_cRef == 0)
	{
		// You can release here or in destructor
		m_pITypeInfo -> Release();
		m_pITypeInfo = NULL;

		delete(this);	// destructor, deletes object
		return(0);
	}
	
	return(m_cRef);
}

// Implementation of IDispatch's Methods
HRESULT CMyMath::GetTypeInfoCount(UINT *pCountTypeInfo)
{
	// as we have type library it is 1, else 0
	*pCountTypeInfo = 1;

	return(S_OK);
}

HRESULT CMyMath::GetTypeInfo(UINT iTypeInfo, LCID lcid, ITypeInfo **ppITypeInfo)	// iTypeInfo = Integer Type Information
{
	// You can have array of ITypeInfo
	// 0 is starting index
	*ppITypeInfo = NULL;

	if(iTypeInfo != 0)
		return(DISP_E_BADINDEX);	// Integer is a Index of Type Info

	// HRESULT values starting with DISP are related to Automation/IDispatch

	m_pITypeInfo -> AddRef();	// Rules of Reference Counting

	*ppITypeInfo = m_pITypeInfo;

	return(S_OK);
}

HRESULT CMyMath::GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
{
	// riid 	 = Is Pre-Defined, Reserved, riid_NULL
	// rgszNames = registered string terminating with zero names, array of function names
	// cNames    = count of Names
	// lcid 	 = locale ID, Eg:- change lcid as per country while OS installation
	// rgDispId  = Array of IDs, Registered Dispatch IDs


	// Call COM Helper Function 
	// no riid and lcid
	// 1st method call method in return, it returns hr
	return(DispGetIDsOfNames(m_pITypeInfo, rgszNames, cNames, rgDispId));
	// Takes first 3 parameters and returns rgDispId

}

// IMP
HRESULT CMyMath::Invoke(DISPID dispidIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
{
	// dispidIdMember   == rgDispId **This is the point of Automation
	// wFlags			=  macros, DISP_METHOD = Function ID  or DISP_PROPERTY = Data Set or Get
	// *pDispParams		=  Paramters of the function which invoke is going to call
	// VARIANT 			=  Great Data Type, converts data types between languages
	// *pVarResults		=  Return Value of the function called by invoke
	// *pExcepInfo		=  Exception Log
	// *puArgErr		=  Error Log

	// variable declarations
	HRESULT hr = S_OK;

	// COM Helper Function
	// no riid and lcid
	// 2nd method declare hr and call method
	hr = DispInvoke(this, m_pITypeInfo, dispidIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);

	return(hr);
}

// All Functions starting with DISP are in oleaut32.lib



// Implementation of IMyMath
// Applications of COM
HRESULT CMyMath::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	*pSum = num1 + num2;
	
	return(S_OK);
}

// Implementation of ISubtract
HRESULT CMyMath::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	*pSubtract = num1 - num2;
	
	return(S_OK);
}

// IMP
// Similar to Initialize Inner Component
// Makes COM Language Independent
HRESULT CMyMath::InitInstance(void)
{
	// function Declarations
	void ComErrorDescriptionString(HWND, HRESULT);

	// variable declarations
	HRESULT  hr 			= S_OK;
	ITypeLib *pITypeLib 	= NULL;

	// code
	if(m_pITypeInfo == NULL)
	{
		hr = LoadRegTypeLib(LIBID_AutomationServer,	// GUID, put in reg file
							 1,						// major version, put in reg file, IDL file
							 0,						// minor version, put in reg file, in IDL file
							 0x00,					// LANG_NEUTRAL, language ID - English
							 &pITypeLib);			// Returns Pointer

		if(FAILED(hr))
		{
			ComErrorDescriptionString(NULL, hr);
			
			return(hr);
		}

		// We Get Type Info Here
		// Type Info of IMyMath stored in m_pITypeInfo
		hr = pITypeLib -> GetTypeInfoOfGuid(IID_IMyMath, &m_pITypeInfo);
	
		if(FAILED(hr))
		{
			ComErrorDescriptionString(NULL, hr);
			
			pITypeLib -> Release();
			pITypeLib = NULL;

			return(hr);
		}

		pITypeLib -> Release();
		pITypeLib = NULL;
	
	}

	return(S_OK);

}


// Implementationof CMyMathClassFactory Constructor
CMyMathClassFactory::CMyMathClassFactory(void)
{
	m_cRef = 1;
	
}

// Implementationof CMyMathClassFactory Destructor
CMyMathClassFactory::~CMyMathClassFactory(void)
{
	// no code
	// destroys object
	
}

// Implementationof CMyMathClassFactory's IClassFactory's IUnknown
HRESULT CMyMathClassFactory::QueryInterface(REFIID riid, void **ppv)
{
	if(riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);
	else if(riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory*>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv) -> AddRef();

	return(S_OK);
	
}

ULONG CMyMathClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef); 		// m_cRef = 2
	
	return(m_cRef);
}

ULONG CMyMathClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1  later becomes 0
	
	if(m_cRef == 0)
	{
		delete(this);	// call to destructor, destroys object
		return(0);	// m_cRef gone from memory
	}
	
	return(m_cRef);
}


// Implementation of CMyMathClassFactory's IClassFactory's Methods
HRESULT CMyMathClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)	// NULL, IID_IMyMath or IID_ISubtract, &pIMyMath or &pISubtract
{
	// variable declarations
	CMyMath *pCMyMath 	= NULL;
	HRESULT hr			= S_OK;
	
	// code
	if(pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);
	
	// create object
	// calls constructor
	pCMyMath = new CMyMath;	// increment

	if(pCMyMath == NULL)
		return(E_OUTOFMEMORY);

	// call to automation related init method
	pCMyMath -> InitInstance();

	if(FAILED(hr))
		return(hr);


	// get the requested interface 
	hr = pCMyMath -> QueryInterface(riid, ppv);	// IID_IMyMath, pIMyMath
	
	pCMyMath -> Release();	// if QueryInterface() fails , CoClass release
	
	return(hr);

}

HRESULT CMyMathClassFactory::LockServer(BOOL fLock)
{
	if(fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	
	return(S_OK);
}



// Implementation of Exported functions from this DllMain
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declarations
	CMyMathClassFactory *pCMyMathClassFactory = NULL;
	HRESULT hr;
	
	// code
	if(rclsid != CLSID_MyMath)
		return(CLASS_E_CLASSNOTAVAILABLE);
	
	if(riid == IID_IClassFactory)
	{
	
		// create class factory
		pCMyMathClassFactory = new CMyMathClassFactory;
	
		if(pCMyMathClassFactory == NULL)
			return(E_OUTOFMEMORY);

		hr = pCMyMathClassFactory -> QueryInterface(riid, ppv);
		
		pCMyMathClassFactory -> Release();
		
		return(hr);
	}

	return (E_NOINTERFACE);

}

extern "C" HRESULT __stdcall DllCanUnloadNow(void)
{
	if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	
	else
		return(S_FALSE);
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

// Optional
// Programatically Register
// DllRegisterServer()
// DllUnregisterServer()










