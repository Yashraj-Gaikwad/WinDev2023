#include <windows.h>
#include "AggregationInnerComponentWithRegFile.h"

// interface declaration (only for internal use)
// making our own IUnknown
interface INoAggregationIUnknown
{
	virtual HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**) 	= 0;

	virtual ULONG   __stdcall AddRef_NoAggregation(void) 					= 0;

	virtual ULONG   __stdcall Release_NoAggregation(void) 					= 0;
};



// class declarations
// implements interfaces
class CMultiplicationDivision:	public INoAggregationIUnknown, IMultiplication, IDivision
{
	private:
		long m_cRef;	// long for reference counting
		
		IUnknown *m_pIunknownOuter;		// IUnknown Outer's Pointer in Inner

	public:
		
		// constructor
		CMultiplicationDivision(IUnknown*);	// allocates memory, initilizes
		
		// destructor
		~CMultiplicationDivision(void);	// deletes or frees memory
		
		// Aggregation Non Supported IUnknown method
		HRESULT __stdcall QueryInterface_NoAggregation(REFIID, void**);
		ULONG   __stdcall AddRef_NoAggregation(void);
		ULONG   __stdcall Release_NoAggregation(void);

		//	Aggregation Supported IUnknown method
		// IUnknown
		HRESULT  __stdcall	QueryInterface(REFIID, void**);		// to get the desired interface
		ULONG	 __stdcall	AddRef(void);						// thread safe increment reference count
		ULONG	 __stdcall	Release(void);						// thread safe decrement reference count

		// IMultiplication
		HRESULT	 __stdcall MultiplicationOfTwoIntegers(int, int, int*);
				
		// IDivision
		HRESULT	 __stdcall DivisionOfTwoIntegers(int, int, int*);
	
};

// Class Factory
// creates objects
class CMultiplicationDivisionClassFactory: public IClassFactory
{
	private:
		long m_cRef;
	
	public:
	// constructor
	CMultiplicationDivisionClassFactory(void);	// allocates memory
	
	// destructor
	~CMultiplicationDivisionClassFactory(void);	// frees memory
	
	// IUnknown
	HRESULT  __stdcall	QueryInterface(REFIID, void**);		// Gets desired interface
	ULONG	 __stdcall	AddRef(void);						// thread safe increment reference count
	ULONG	 __stdcall	Release(void);						// thread safe decrement reference count
	
	// Class Factory
	HRESULT	__stdcall CreateInstance(IUnknown*, REFIID, void**);	// Creates objects
	HRESULT	__stdcall LockServer(BOOL);								// life time management of objects
	
	
};


// global variables
long glNumberOfActiveComponents = 0;	// no. of active components
long glNumberOfServerLocks		= 0;	// no. of locks on dll

// Dll Main
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



// Implementation of CMultiplicationDivision

// constructor
CMultiplicationDivision::CMultiplicationDivision(IUnknown *pIUnkownOuter)
{
	m_cRef = 1;											// hardcoded initialization
	InterlockedIncrement(&glNumberOfActiveComponents);	// thread safe global increment
	
	// 4 Romantic Lines
	if(pIUnkownOuter != NULL)	// Outer exists, Aggregation
		m_pIunknownOuter = pIUnkownOuter;
	else	// pIUnkownOuter == NULL, No Aggregation
		m_pIunknownOuter = reinterpret_cast<IUnknown*> (static_cast<INoAggregationIUnknown *>(this));

}


// destructor
CMultiplicationDivision::~CMultiplicationDivision(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);	// thread safe global decrement
	
	if(m_pIunknownOuter)
	{
		m_pIunknownOuter -> Release();
		m_pIunknownOuter = NULL;
	}

}

// No Aggregation IUnknown
// Method Coloring
HRESULT CMultiplicationDivision::QueryInterface_NoAggregation(REFIID riid, void **ppv)
{
	if(riid == IID_IUnknown)
		*ppv = static_cast<INoAggregationIUnknown *>(this);
	
	else if(riid == IID_IMultiplication)
		*ppv = static_cast<IMultiplication *>(this);
	
	else if(riid == IID_IDivision)
		*ppv = static_cast<IDivision *>(this);
	
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);	// Error No Interface
	}
	
	// Used for non inheritance interface uncommon or unrelated, use with caution
	reinterpret_cast<IUnknown*> (*ppv) -> AddRef();	// calling Aggregation IUnknown's AddRef, Method Colouring
	
	return(S_OK);	// Successful Return
	


}

ULONG CMultiplicationDivision::AddRef_NoAggregation(void)
{
	InterlockedIncrement(&m_cRef);

	return(m_cRef);
}

ULONG CMultiplicationDivision::Release_NoAggregation(void)
{
	InterlockedIncrement(&m_cRef);

	if(m_cRef == 0)
	{
		delete(this);
	
		return(0);
	}
	
	return(m_cRef);
}


// IUnknown Aggregation

// QueryInterface
HRESULT CMultiplicationDivision::QueryInterface(REFIID riid, void **ppv)
{
	// call to outer object's QueryInterface
	return(m_pIunknownOuter -> QueryInterface(riid, ppv));	// returns to client

}

// AddRef
ULONG CMultiplicationDivision::AddRef(void)
{
	// Call to outer object's AddRef
	return(m_pIunknownOuter -> AddRef());

}


// Release
ULONG CMultiplicationDivision::Release(void)
{
	// Call to outer object's release
	return(m_pIunknownOuter -> Release());

}


// IMultiplication
HRESULT CMultiplicationDivision::MultiplicationOfTwoIntegers(int num1, int num2, int *pMultiplication)
{
	*pMultiplication = num1 * num2;
	
	return(S_OK);
	
}

// IDivision
HRESULT CMultiplicationDivision::DivisionOfTwoIntegers(int num1, int num2, int *pDivision)
{
	*pDivision = num1 / num2;
	
	
	return(S_OK);
}


// Implementation of CMultiplicationDivisionClassFactory

// constructor
CMultiplicationDivisionClassFactory::CMultiplicationDivisionClassFactory(void)
{
	m_cRef = 1;
	
}

// destructor
CMultiplicationDivisionClassFactory::~CMultiplicationDivisionClassFactory(void)
{
	
	// no code
	
}


// IUnknown

// IClassFactory
HRESULT CMultiplicationDivisionClassFactory::QueryInterface(REFIID riid, void **ppv)
{
		// if else if ladder
	// Interface Inheritance
	if(riid == IID_IUnknown)
		*ppv = static_cast<IClassFactory *>(this);
	
	else if(riid == IID_IClassFactory)
		*ppv = static_cast<IClassFactory *>(this);
	
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);	// Error No Interface
	}
	
	// Used for non inheritance interface uncommon or unrelated, use with caution
	reinterpret_cast<IUnknown*> (*ppv) -> AddRef();
	
	return(S_OK);	// Successful Return
	
	
	
}

ULONG CMultiplicationDivisionClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef);
	return(m_cRef);
	
}

ULONG CMultiplicationDivisionClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);
	
	if(m_cRef == 0)
	{
		delete(this);
		
		return(0);
	}
	
	return(m_cRef);
}

// ClassFactory

HRESULT CMultiplicationDivisionClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv) 
// reinterpret_cast<IUnknown *>(this), IID_IUnknown, (void**)&m_pIUnknownInner
{
	CMultiplicationDivision *pCMultiplicationDivision = NULL;
	
	HRESULT hr = S_OK;
	
	if((pUnkOuter != NULL) && (riid != IID_IUnknown))
		return(CLASS_E_NOAGGREGATION);
	
	// For Aggregation, pUnkOuter == pointer to Outer IUnknown and riid == IID_IUnknown

	// create object
	pCMultiplicationDivision = new CMultiplicationDivision(pUnkOuter);
	
	if(pCMultiplicationDivision == NULL)
		return(E_OUTOFMEMORY);
	
	hr = pCMultiplicationDivision -> QueryInterface_NoAggregation(riid, ppv);
	
	pCMultiplicationDivision -> Release_NoAggregation();
	
	return(hr);
}

HRESULT CMultiplicationDivisionClassFactory::LockServer(BOOL fLock)
{
	if(fLock)
		InterlockedIncrement(&glNumberOfServerLocks);
	
	else
		InterlockedDecrement(&glNumberOfServerLocks);
	
	return(S_OK);
}

// Implementation of Exported functions from this dll
HRESULT __stdcall DllGetClassObject(REFCLSID rclsid, REFIID riid, void **ppv)
{
	// variables
	CMultiplicationDivisionClassFactory *pCMultiplicationDivisionClassFactory = NULL;
	
	HRESULT hr 	= S_OK;
	
	if(rclsid != CLSID_MultiplicationDivision)
		return(CLASS_E_NOAGGREGATION);
	
	// create class factory object
	pCMultiplicationDivisionClassFactory = new CMultiplicationDivisionClassFactory;
	
	if(pCMultiplicationDivisionClassFactory == NULL)
		return(E_OUTOFMEMORY);
	
	hr = pCMultiplicationDivisionClassFactory -> QueryInterface(riid, ppv);
	
	pCMultiplicationDivisionClassFactory -> Release();
	
	return(hr);
	
	
	
}

HRESULT __stdcall DllCanUnloadNow(void)
{
	if((glNumberOfActiveComponents == 0) && (glNumberOfServerLocks == 0))
		return(S_OK);
	
	else
		return(S_FALSE);
	
}







