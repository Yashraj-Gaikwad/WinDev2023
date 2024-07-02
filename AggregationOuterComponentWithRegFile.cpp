#include <windows.h>
#include "AggregationInnerComponentWithRegFile.h"
#include "AggregationOuterComponentWithRegFile.h"
#define UNICODE

// class declarations
class CSumSubtract : public ISum, ISubtract
{
private:
	long 			 m_cRef;		// for reference counting
	IUnknown		 *m_pIUnknownInner;

public:
	// constructor
	CSumSubtract(void);
	
	// destructor
	~CSumSubtract(void);
	
	// IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void **);	// REFIID = Reference of Interface Identifers, void** = double indirection
	ULONG   __stdcall AddRef(void);
	ULONG	__stdcall Release(void);
	
	// ISum methods
	HRESULT __stdcall SumOfTwoIntegers(int, int, int *);
	
	// ISubtract methods
	HRESULT __stdcall SubtractionOfTwoIntegers(int, int, int *);
	
	// Inner Component Creation
	HRESULT __stdcall InitializeInnerComponent(void);
	
};

class CSumSubtractClassFactory :public IClassFactory
{
private:
	long m_cRef;

public:

	// constructor
	CSumSubtractClassFactory(void);
	
	//destructor
	~CSumSubtractClassFactory(void);
	
	// IUnknown methods
	HRESULT __stdcall QueryInterface(REFIID, void **);
	ULONG	__stdcall AddRef(void);
	ULONG	__stdcall Release(void);
	
	// IClassFactory methods
	HRESULT __stdcall CreateInstance(IUnknown *, REFIID, void **);
	HRESULT __stdcall LockServer(BOOL);
	
};

// global variable declarations
long glNumberOfActiveComponents = 0;	// no. of active components
long glNumberOfServerLocks = 0;			// no. of locks on this dll

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


// Implementation of CSumSubtract Constructor
CSumSubtract::CSumSubtract(void)
{
	// hardcoded initialization
	m_cRef 				= 1;
	m_pIUnknownInner	= NULL;
	
	// Thread safe atomic operation
	InterlockedIncrement(&glNumberOfActiveComponents);	// increment global counter
}

// Implementation CSumSubtract Destructor
CSumSubtract::~CSumSubtract(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);	// decrement global counter
	
	// Release Interface Pointers
	
	if(m_pIUnknownInner)
	{
		m_pIUnknownInner -> Release();
		m_pIUnknownInner = NULL;
	}
	
}

// Implementation of CSumSubtract IUnknown
HRESULT CSumSubtract::QueryInterface(REFIID riid, void **ppv)
{
	// runtime type identification
	
	if(riid == IID_IUnknown)
		*ppv = static_cast<ISum *>(this);	// compile time

	else if(riid == IID_ISum)				// we want this
		*ppv = static_cast<ISum *>(this);

	else if(riid == IID_ISubtract)
		*ppv = static_cast<ISubtract *>(this);
	
	else if(riid == IID_IMultiplication)
		return(m_pIUnknownInner -> QueryInterface(riid, ppv));		// Calls to Inner's INoAggregation QueryInterface

	
	// Double googly
	// Calls to Inner's INoAggregation QueryInterface
	else if(riid == IID_IDivision)
		return(m_pIUnknownInner -> QueryInterface(riid, ppv));	// returns to client

	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv) -> AddRef();	// whoever you are call the addref inside your iunknown

	return(S_OK);
	
}

ULONG CSumSubtract::AddRef(void)
{
	InterlockedIncrement(&m_cRef);	// 
	
	return(m_cRef);
}

ULONG CSumSubtract::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1 later 0 after pISum and pSubtract release
	
	if(m_cRef == 0)
	{
		delete(this);	// destructor, deletes object
		return(0);
	
	}
	
	return(m_cRef);
}

// Implementation of ISum
HRESULT CSumSubtract::SumOfTwoIntegers(int num1, int num2, int *pSum)
{
	*pSum = num1 + num2;
	
	return(S_OK);
}

// Implementation of ISubtract
HRESULT CSumSubtract::SubtractionOfTwoIntegers(int num1, int num2, int *pSubtract)
{
	*pSubtract = num1 - num2;
	
	return(S_OK);
}

HRESULT CSumSubtract::InitializeInnerComponent(void)
{
	// variable declarations
	HRESULT hr;
	
	hr = CoCreateInstance(CLSID_MultiplicationDivision, 	// Only change clsid to implement another inner
						  reinterpret_cast<IUnknown *>(this), 	// Outer giving its IUnknown to Innner, Not NULL for Aggregation
						  CLSCTX_INPROC_SERVER, 
						  IID_IUnknown, 
						  (void**)&m_pIUnknownInner);	// Outer taking Inner's IUnknown, Point Of Aggregation
	
	// m_pIUnknownInner == IUnknownNoAggregation

	if(FAILED(hr))
	{
		MessageBox(NULL, TEXT("IUnknown Interface can not be obtained from inner component"), TEXT("Com Error"), MB_OK);
		
		m_pIUnknownInner -> Release();
		m_pIUnknownInner = NULL;

		return(E_FAIL);
	}
	
	
	return(S_OK);
}

// Implementationof CSumSubtractClassFactory Constructor
CSumSubtractClassFactory::CSumSubtractClassFactory(void)
{
	m_cRef = 1;
	
}

// Implementationof CSumSubtractClassFactory Destructor
CSumSubtractClassFactory::~CSumSubtractClassFactory(void)
{
	// no code
	// destroys object
	
}

// Implementationof CSumSubtractClassFactory's IClassFactory's IUnknown
HRESULT CSumSubtractClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CSumSubtractClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef); 		// m_cRef = 2
	
	return(m_cRef);
}

ULONG CSumSubtractClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1  later becomes 0
	
	if(m_cRef == 0)
	{
		delete(this);	// call to destructor, destroys object
		return(0);	// m_cRef gone from memory
	}
	
	return(m_cRef);
}


// Implementation of CSumSubtractClassFactory's IClassFactory's Methods
HRESULT CSumSubtractClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)	
												// NULL, IID_ISum or IID_ISubtract, &pISum or &pISubtract
{
	// variable declarations
	CSumSubtract *pCSumSubtract = NULL;
	HRESULT hr = S_OK;
	
	// code
	if(pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION); // No Aggregation between Client and Outer
	
	// create object
	// calls constructor
	pCSumSubtract = new CSumSubtract;	// increment

	if(pCSumSubtract == NULL)
		return(E_OUTOFMEMORY);

	// initialize inner component
	hr = pCSumSubtract -> InitializeInnerComponent(); 	// Inner dll loaded
	
	if(FAILED(hr))
	{
		MessageBox(NULL, TEXT("Failed to initialize inner component"), TEXT("Com Error"), MB_OK);
		
		pCSumSubtract -> Release();
		
		return(E_FAIL);
	}
	
	// get interface
	hr = pCSumSubtract -> QueryInterface(riid, ppv);
	
	pCSumSubtract -> Release();
	
	return(hr);

}

HRESULT CSumSubtractClassFactory::LockServer(BOOL fLock)
{
	if(fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	
	return(S_OK);
}


// Implementation of Exported functions from this DllMain
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variable declarations
	CSumSubtractClassFactory *pCSumSubtractClassFactory = NULL;
	HRESULT hr;
	
	// code
	if(rclsid != CLSID_SumSubtract)
		return(CLASS_E_CLASSNOTAVAILABLE);
	
	if(riid == IID_IClassFactory)
	{
	
		// create class factory
		pCSumSubtractClassFactory = new CSumSubtractClassFactory;
	
		if(pCSumSubtractClassFactory == NULL)
			return(E_OUTOFMEMORY);

		hr = pCSumSubtractClassFactory -> QueryInterface(riid, ppv);
		
		pCSumSubtractClassFactory -> Release();
		
		return(hr);
	}

	return (E_NOINTERFACE);

}

HRESULT __stdcall DllCanUnloadNow(void)
{
	if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	
	else
		return(S_FALSE);
}















