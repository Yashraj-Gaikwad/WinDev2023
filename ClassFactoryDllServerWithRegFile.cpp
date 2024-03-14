#define UNICODE
#include <windows.h>
#include <math.h>
#include "ClassFactoryDllServerWithRegFile.h"

// class declarations
class CSineWave : public ICircle, ISineWave
{
private:
	long m_cRef;		// for reference counting

public:
	// constructor
	CSineWave(void);
	
	// destructor
	~CSineWave(void);
	
	// IUnknown specific method declarations
	HRESULT __stdcall QueryInterface(REFIID, void **);	// REFIID = Reference of Interface Identifers, void** = double indirection
	ULONG   __stdcall AddRef(void);
	ULONG	__stdcall Release(void);
	
	// ICircle methods
	HRESULT __stdcall PointOnCircle(double, double, int*, int*);
	
	// ISineWave methods
	HRESULT __stdcall PointOnSineWave(double, double, double, int*, int*);
	
};

class CSineWaveClassFactory :public IClassFactory
{
private:
	long m_cRef;

public:

	// constructor
	CSineWaveClassFactory(void);
	
	//destructor
	~CSineWaveClassFactory(void);
	
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


// Implementation of CSineWave Constructor
CSineWave::CSineWave(void)
{
	// hardcoded initialization
	m_cRef = 1;
	
	// Thread safe atomic operation
	InterlockedIncrement(&glNumberOfActiveComponents);	// increment global counter
}

// Implementation CSineWave Destructor
CSineWave::~CSineWave(void)
{
	InterlockedDecrement(&glNumberOfActiveComponents);	// decrement global counter
	
}

// Implementation of CSineWave IUnknown
HRESULT CSineWave::QueryInterface(REFIID riid, void **ppv)
{
	// runtime type identification
	
	if(riid == IID_IUnknown)
		*ppv = static_cast<ICircle *>(this);	// compile time
	else if(riid == IID_ICircle)				// we want this
		*ppv = static_cast<ICircle *>(this);
	else if(riid == IID_ISineWave)
		*ppv = static_cast<ISineWave *>(this);
	else
	{
		*ppv = NULL;
		return(E_NOINTERFACE);
	}

	reinterpret_cast<IUnknown*>(*ppv) -> AddRef();	// whoever you are call the addref inside your iunknown

	return(S_OK);
	
}

ULONG CSineWave::AddRef(void)
{
	InterlockedIncrement(&m_cRef);	// 
	return(m_cRef);
}

ULONG CSineWave::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1 later 0 after pICircle and pSubtract release
	
	if(m_cRef == 0)
	{
		delete(this);	// destructor, deletes object
		return(0);
	
	}
	
	return(m_cRef);
}

// Implementation of ICircle
// radius, angle, x, y
HRESULT CSineWave::PointOnCircle(double radius, double angle, int* x, int* y)
{
	*x = (int)(radius * cos(angle));
	*y = (int)(radius * sin(angle));
	
	return(S_OK);
}

// Implementation of ISineWave
// amplitutde, angle, x, y
HRESULT CSineWave::PointOnSineWave(double amplitude, double angle, double phase, int* x, int* y)
{
	*x = *x + 1;
	*y = (int)(amplitude * sin(angle - phase));
	
	return(S_OK);
}

// Implementationof CSineWaveClassFactory Constructor
CSineWaveClassFactory::CSineWaveClassFactory(void)
{
	m_cRef = 1;
	
}

// Implementationof CSineWaveClassFactory Destructor
CSineWaveClassFactory::~CSineWaveClassFactory(void)
{
	// no code
	// destroys object
	
}

// Implementationof CSineWaveClassFactory's IClassFactory's IUnknown
HRESULT CSineWaveClassFactory::QueryInterface(REFIID riid, void **ppv)
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

ULONG CSineWaveClassFactory::AddRef(void)
{
	InterlockedIncrement(&m_cRef); 		// m_cRef = 2
	
	return(m_cRef);
}

ULONG CSineWaveClassFactory::Release(void)
{
	InterlockedDecrement(&m_cRef);	// m_cRef = 1  later becomes 0
	
	if(m_cRef == 0)
	{
		delete(this);	// call to destructor, destroys object
		return(0);	// m_cRef gone from memory
	}
	
	return(m_cRef);
}


// Implementation of CSineWaveClassFactory's IClassFactory's Methods
HRESULT CSineWaveClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)	// NULL, IID_ICircle or IID_ISineWave, &pICircle or &pISineWave
{
	// variable declarations
	CSineWave *pCSineWave = NULL;
	HRESULT hr;
	
	// code
	if(pUnkOuter != NULL)
		return(CLASS_E_NOAGGREGATION);
	
	// create object
	// calls constructor
	pCSineWave = new CSineWave;	// increment

	if(pCSineWave == NULL)
		return(E_OUTOFMEMORY);

	// get the requested interface 
	hr = pCSineWave -> QueryInterface(riid, ppv);	// IID_ICircle, pICircle
	
	pCSineWave -> Release();	// if QueryInterface() fails , CoClass release
	
	return(hr);

}

HRESULT CSineWaveClassFactory::LockServer(BOOL fLock)
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
	CSineWaveClassFactory *pCSineWaveClassFactory = NULL;
	HRESULT hr;
	
	// code
	if(rclsid != CLSID_CSineWave)
		return(CLASS_E_CLASSNOTAVAILABLE);
	
	if(riid == IID_IClassFactory)
	{
	
		// create class factory
		pCSineWaveClassFactory = new CSineWaveClassFactory;
	
		if(pCSineWaveClassFactory == NULL)
			return(E_OUTOFMEMORY);

		hr = pCSineWaveClassFactory -> QueryInterface(riid, ppv);
		
		pCSineWaveClassFactory -> Release();
		
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















