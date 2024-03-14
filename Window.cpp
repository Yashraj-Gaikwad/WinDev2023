/*
Wave Simulator Project
By Yashraj Abhay Gaikwad at AstroMediComp
Using C and C++, Win32 SDK, COM
GUI programming

1) Win32 GUI - colors and changes, 3 dialog box
2) COM - 2 Interfaces
3) SAPI - 2 voices
4) Spot Validation
5) File Handling
6) String Manipulation

*/

// header files
#include <windows.h>	// for win32 functions
#include <math.h>		// for sine functions
#include <sapi.h>		// for speech functions
#include <stdio.h>		// for file IO
#include <stdlib.h>		// for IO
#include <sphelper.h>	// For SAPI and speech helper functions beginning with sp
#include <string.h>		// For string manipulation
#include <assert.h>
#include <windowsx.h>

#include "Window.h"
#include "Dialogbox.h"
#include "AboutDlgBox.h"
#include "RegisterDlgBox.h"
#include "ClassFactoryDllServerWithregFile.h"

#define UNICODE


// global variables
double dAngle 				= 0.0;
double dAmplitude 			= 50.0;
double dFrequency 			= 5.0;

COLORREF currentColor 		= RGB(255, 128, 0);	// default color orange

HWND ghWnd = NULL;
HINSTANCE hInst;
HRESULT hr = S_OK;
RECT		rect;

int num_points = 500;
int WAVE_COUNT = 1;

wchar_t* TextToSpeech;
ULONG numVoices = 0L;
wchar_t* DescriptionString = NULL;
CSpStreamFormat audioFormat;
char textBuffer[TEXT_FILE_NUM_CHARS];
WCHAR textBuffer_WideChar[TEXT_FILE_NUM_CHARS];

BOOL bSoundOn 				= FALSE;
BOOL bUserRegistered 		= FALSE;
BOOL bConvertTextToSpeech 	= FALSE;
BOOL bCheckBoxAudioLog		= FALSE;

HWND hWnd;
HWND hWndDlg;
HWND hWndRegisterDlg;
HWND hWndAboutDlg;

TCHAR 	szClassName[] = TEXT("Sine Wave");

POINT iPlotPoint[NUM_POINTS];
POINT iFinalPlotPoint[NUM_POINTS];

	

// Sapi Interfaces
ISpVoice 			*pISpVoice 				= NULL;
IEnumSpObjectTokens *pIEnumSpObjectTokens 	= NULL;
ISpObjectToken 		*pISpObjectToken 		= NULL;
ISpStream			*pISpStream				= NULL;

FILE 				*file 					= NULL;

SYSTEMTIME SysTime;
char FileName[255];
char FileNameForAudio[255];


// COM Interface
ISineWave	*pISineWave = NULL;
ICircle 	*pICircle	= NULL;

// function declarations
void SafeInterfaceRelease(void);
void convertTextToSpeech();


// Global Callback Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK MyDlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK AboutDlgProc(HWND, UINT, WPARAM, LPARAM);

BOOL CALLBACK RegisterDlgProc(HWND, UINT, WPARAM, LPARAM);

/* callback function - we cant decide return type, it is os dependent
 1) return type is os specified 
 2) you cant decide the no.,types, order of parameters, it os specified
 3) you cant decide the calling convention, it is os specific

*/

// Entry point function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declarations
	WNDCLASSEX wndclass;
	HWND hWnd;
	MSG msg;
	HRESULT hr = S_OK;
	
	hr = CoInitialize(NULL);
	
	if(FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Failed"), szClassName, MB_OK | MB_ICONERROR);
	}

	
	
	// code
	// 12 members
	wndclass.cbSize		 	= sizeof(WNDCLASSEX);
	wndclass.style 			= CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra 	= 0;
	wndclass.cbWndExtra 	= 0;
	wndclass.hInstance 		= hInstance;
	wndclass.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	
	// change
	wndclass.hIcon 			= LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));				// IDI - id of default icon
	wndclass.hIconSm 		= LoadIcon(hInstance, MAKEINTRESOURCE(YAG_ICON));

	wndclass.hCursor 		= LoadCursor(NULL, IDC_ARROW);
	wndclass.lpfnWndProc 	= WndProc;
	wndclass.lpszClassName 	= szClassName;
	wndclass.lpszMenuName 	= NULL;


	// Register window class
	ATOM bRet = RegisterClassEx(&wndclass);		// returns atomic string

	// error checking
	assert(bRet);
	

	// Create the window
	// 11 parameters
	hWnd = CreateWindow(szClassName,
		TEXT("Real Time Sine Wave Simulator"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		700,
		700,
		NULL,
		NULL,
		hInstance,
		NULL);		// LPCREATESTRUCT - long pointer to createstruct
	
	// error checking
	if(hWnd == NULL)
	{
		MessageBox(NULL,TEXT("Creat Window Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
			
		exit(-1);
	}
	
	ghWnd = hWnd;

	// Show the window
	ShowWindow(hWnd, SW_NORMAL);

	// Update the window
	UpdateWindow(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HDC 		hdc		= NULL;
	PAINTSTRUCT ps;
	HBRUSH 		hBrush  = NULL;
	static RECT rect;
	static int center_x, center_y = 0;
	static int point_x, point_y   = 0;
	static int x, y = 0;
	static BOOL bReset = TRUE;
	int i;
	double phase = 0;
	
	
	HRESULT hr = S_OK;
	
	
	//code
	switch (iMsg)
	{
		case WM_CREATE:
			
			// COM loads Library
			hr = CoCreateInstance(CLSID_CSineWave,		// GUID of class CSineWave
				NULL, 									// NULL for no aggregation
				CLSCTX_INPROC_SERVER,					// Class Context in process server (.exe)
				IID_ISineWave,								// IID of ISineWave
				(void **)&pISineWave);						// pointer to store interface pointer						
				// virtual table to implementation table
			
			if(FAILED(hr))
			{
				MessageBox(hWnd, TEXT("ISineWave Interface Can Not Be Obtained"), TEXT("Com Error"), MB_OK);
				DestroyWindow(hWnd);
			}

			
			hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_INPROC_SERVER, IID_ISpVoice, (void **)&pISpVoice);
		
			if(FAILED(hr))
			{
				MessageBox(NULL, TEXT("COM Failed"), szClassName, MB_OK | MB_ICONERROR);
			
				DestroyWindow(hWnd);
			}
		
			hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &pIEnumSpObjectTokens);	// SP CATID - speech category id
			
			if(FAILED(hr))
			{
				MessageBox(NULL, TEXT("SpEnumTokens Failed"), szClassName, MB_OK | MB_ICONERROR);
				
				DestroyWindow(hWnd);
			}

			// From this list of voice tokens, get the no. of available voices	
			hr = pIEnumSpObjectTokens -> GetCount(&numVoices);

			if(FAILED(hr))
			{
				MessageBox(NULL, TEXT("GetCount Failed"), szClassName, MB_OK | MB_ICONERROR);
			}

			// Assigning audio format
			audioFormat.AssignFormat(SPSF_44kHz16BitStereo);	// Best and default audio wav format 44.1 KHz 16-bit Stereo

			center_y = (rect.bottom - rect.top) / 2;
			center_x = (rect.right - rect.left) / 2;
		
			for(i = 0; i < NUM_POINTS; i++)
			{	
				// Animating Multiple Points
				iPlotPoint[i].x 		= 0;
				iPlotPoint[i].y 		= 0;
				iFinalPlotPoint[i].x 	= 0;
				iFinalPlotPoint[i].y 	= 0;

			}

			for(i = 0; i < NUM_POINTS; i++)
			{
				iPlotPoint[i].x = -(i * POINT_SIZE);
			}

			
			GetClientRect(hWnd, &rect);

			//x = rect.left;

			SetTimer(hWnd, YAG_TIMER, TIME_INTERVAL_IN_MS, NULL);	// null - timerproc
			
		

		break;

		case WM_PAINT:

			hdc = BeginPaint(hWnd, &ps);

			if(bReset == TRUE)
			{
				
				point_y = center_y + y;

			}

			hBrush = CreateSolidBrush(currentColor);	// orange

			SelectObject(hdc, hBrush);
			
			// All window coordinates are in integer
			// Call server here, to get x and y
			
			for(int j = 0; j < WAVE_COUNT; j++)
			{
			
				for(i = 0; i < NUM_POINTS; i++)
				{
					phase = (double)i / dFrequency;
				
					pISineWave -> PointOnSineWave(dAmplitude, dAngle, phase, (int*)&iPlotPoint[i].x, (int*)&iPlotPoint[i].y);
				
					// Animating Multiple Sine Waves
					iFinalPlotPoint[i].x = iPlotPoint[i].x;
					iFinalPlotPoint[i].y = center_y + iPlotPoint[i].y + (j * WAVE_LEVEL_Y_MULTIPLE);

					Ellipse(hdc, iFinalPlotPoint[i].x, iFinalPlotPoint[i].y, iFinalPlotPoint[i].x + POINT_SIZE, iFinalPlotPoint[i].y + POINT_SIZE);
				
				}

			}
			
			if(hBrush)
			{
				DeleteObject(hBrush);
				hBrush = NULL;
			}

			if(hdc)
			{
				EndPaint(hWnd, &ps);
				hdc = NULL;

			}	


		break;

		case WM_TIMER:
			// Animation ends
			KillTimer(hWnd, YAG_TIMER);
			
			if(x < (rect.right + POINT_SIZE))
				dAngle = dAngle + 0.15;
			
			
			
			// Repaint Update
			InvalidateRect(hWnd, NULL, TRUE); 	// calls WM_PAINT

			// Set timer for next animation
			SetTimer(hWnd, YAG_TIMER, TIME_INTERVAL_IN_MS, NULL);

		break;



		case WM_KEYDOWN:

			switch(LOWORD(wParam))
			{
				case VK_ESCAPE:
				
					DestroyWindow(hWnd);
				
				break;
			
			
				case VK_SPACE:
					
					// Call GetWindowLongPtr()
					hInst = ((HINSTANCE) (GetWindowLongPtr(hWnd, GWLP_HINSTANCE)));
					
					// Call DialogBox
					DialogBox(hInst, TEXT("SELECTCOLOUR"), hWnd, ((DLGPROC) (MyDlgProc)));
				
					InvalidateRect(hWnd, NULL, TRUE);
				
				break;


				default:

				break;

			}


		break;


		case WM_SIZE:
			
			GetClientRect(hWnd, &rect);

			x = rect.left;

			center_x = (rect.right - rect.left) / 2;

			center_y = (rect.bottom - rect.top) / 2;


		break;



		case WM_DESTROY:
			
			// Yes or No
			MessageBox(hWnd, TEXT("Do you really want to exit the application?"), TEXT("Message:"), MB_YESNO |MB_ICONQUESTION);
			
			if(file)
			{
				GetLocalTime(&SysTime);
				fprintf(file, "User exited the application at  %02d:%02d:%02d \n", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
				
			}

			
			if(bCheckBoxAudioLog)
			{
				fclose(file);
			    
				// Export Audio
				convertTextToSpeech();
				
			}
			
			SafeInterfaceRelease();
			
			PostQuitMessage(0);
		
		break;

		default:
		
		break;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

void convertTextToSpeech()
{
	// Open File for text to wave functionality in read mode
	fopen(FileName, "r");
	

	if(file == NULL)
	{
		MessageBox(NULL, TEXT("File Open Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
		exit(0);
	}
	
	
	// By default the TTS(TextToSpeech) audio output stream is given to / or bound with the default
	// audio output devices which are the speakers.
	// Now we will give or bind the TTS audio output stream to wave file.
	hr = SPBindToFile(FileNameForAudio,						// audio file name
						SPFM_CREATE_ALWAYS, 			// Create a new file
						&pISpStream,					// Return stream in stream pointer
						&audioFormat.FormatId(),		// Audio format Id
						audioFormat.WaveFormatExPtr());	//	Audio Format Pointers
															// SPEventEnum - default parameter
			
	// Set the stream to the above created stream
	hr = pISpVoice -> SetOutput(pISpStream, FALSE);
			
	if(FAILED(hr))
	{
		MessageBox(NULL, TEXT("SetOutput Failed"), szClassName, MB_OK | MB_ICONERROR);
				
		exit(0);
	}

	
	// Read from the file myself.txt into the text buffer array
	fread(&textBuffer, sizeof(char), TEXT_FILE_NUM_CHARS, file);

	// After the above call to fread we will have the contents of the file in ANSII char text buffer,
	// But speak requires UNICODE or wide char
	// So convert ANSII char text buffer to wide char
	MultiByteToWideChar(CP_UTF8, 0, textBuffer, sizeof(char) * TEXT_FILE_NUM_CHARS, textBuffer_WideChar, TEXT_FILE_NUM_CHARS);

	
	
	// From the list of voice Tokens get the next voice tokens
	pIEnumSpObjectTokens -> Next(1,					// How many tokens
								&pISpObjectToken,	// Base addr of array
								NULL);				// Length of array		
				
	// Get required textual information (name of the voice) from the above token
	SpGetDescription(pISpObjectToken, &DescriptionString);
				
				
	// Set the voice
	pISpVoice -> SetVoice(pISpObjectToken);
	
	
		
	// Convert Text to Speech in the voice set in the above set
	// 2nd param - for flags
	// 3rd param - should I return stream
	pISpVoice -> Speak(textBuffer_WideChar, 0, NULL);
	
}

void toggleWaveParameters(HWND hWndWaveParam, BOOL bFlag)
{

	EnableWindow(GetDlgItem(hWndWaveParam, ID_REG_USER), bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_VOL_ON), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_VOL_OFF), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_RED), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_GREEN), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_BLUE), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_YELLOW), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_MAGENTA), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_CYAN), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_GRAY), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_WHITE), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_VIOLET), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_ORANGE), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_INC_PTS), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_DEC_PTS), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_INC_AMP), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_DEC_AMP), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_INC_FRQ), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_DEC_FRQ), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_INC_WAV), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_DEC_WAV), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_RST_WAV), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, ID_LISTBOX), !bFlag);
	EnableWindow(GetDlgItem(hWndWaveParam, IDCB_EXPAUDCB), !bFlag);
	//EnableWindow(GetDlgItem(hWndWaveParam, ID_), !bFlag);


}



// Dialog Box Procedure
BOOL CALLBACK MyDlgProc(HWND hWndDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HDC 		hdc		= NULL;
	PAINTSTRUCT ps;
	HBRUSH 		hBrush  = NULL;
	static int i,j;
	HICON		hIcon;
	HDC 		hStcCol	= NULL;
	wchar_t 	voice[50];
	TCHAR 		str[255];

	int ispaceCount = 0;
	int countChar  = 0;
	int index = 0;
	int iCheckAudioLog = 0;

	HWND hList;
	HWND hButton;

	// function declaration
	void toggleWaveParameters(HWND, BOOL);


	switch(iMsg)
	{
		
		case WM_CTLCOLORSTATIC:
		
			hStcCol = ((HDC)wParam);
			SetTextColor(hStcCol, RGB(0,0,0));
			SetBkColor(hStcCol, RGB(102,255,255));	// light blue
			return (INT_PTR)CreateSolidBrush(RGB(102,255,255));
			
		break;
		
		
		
		case WM_INITDIALOG:
			
			// Initialization code here
			
			if(bUserRegistered)
			{
				toggleWaveParameters(hWndDlg, FALSE);

				
			}

			else if(!bUserRegistered)
			{
				toggleWaveParameters(hWndDlg, TRUE);

				bUserRegistered = TRUE;
			}


			hButton = GetDlgItem(hWndDlg, ID_VOL_OFF);
			hList = GetDlgItem(hWndDlg, ID_LISTBOX);
			
			SendMessage(hButton, BM_SETCHECK, 1, 0);

			while(numVoices > 0)
			{	
				// From the list of voice Tokens get the next voice tokens
				pIEnumSpObjectTokens -> Next(1,					// How many tokens
										&pISpObjectToken,	// Base addr of array
										NULL);				// Length of array		
				
				// Get required textual information (name of the voice) from the above token
				SpGetDescription(pISpObjectToken, &DescriptionString);

				for(int i = 0; i <= wcslen(DescriptionString); i++)
				{
					voice[i] = DescriptionString[i];

					if(DescriptionString[i] == L' ')
					{
						ispaceCount++;

						if(ispaceCount == 2)
						{
							break;
						}
					}

				}
			
				SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)(voice));
				
				ispaceCount = 0;

				numVoices--;
			}
			
			// reset
			numVoices = 2;
			pIEnumSpObjectTokens -> Reset();

			// Default Selection - Microsoft David
			SendMessage(hList, LB_SETCURSEL, 0, 0);
			
		return TRUE;
		
		case WM_CTLCOLORDLG:

			// Set the background color of dialog box - cyan
			return ((INT_PTR)CreateSolidBrush(MY_DIALOG_BG_COLOR));

		
		return (INT_PTR)CreateSolidBrush(MY_DIALOG_BG_COLOR); // Return the color brush
    
		


		case WM_COMMAND:
			
			switch(LOWORD(wParam))
			{
				case IDOK:
				
				return TRUE;
				
				case IDCANCEL:
					
					
					// 2nd parameter is used as return value of Dialog Box
					EndDialog(hWndDlg, 0);
					
				return TRUE;
				
				case ID_REG_USER:
					
					
				
					// Call GetWindowLongPtr()
					hInst = ((HINSTANCE) (GetWindowLongPtr(hWndDlg, GWLP_HINSTANCE)));
					
					// Call DialogBox
					DialogBox(NULL, TEXT("DATAENTRY"), hWndDlg, ((DLGPROC) (RegisterDlgProc)));

					if (bUserRegistered)
					{
						toggleWaveParameters(hWndDlg, FALSE);

						
					}

					else if (!bUserRegistered)
					{
						toggleWaveParameters(hWndDlg, TRUE);

						bUserRegistered = TRUE;
					}

					InvalidateRect(hWndDlg, NULL, TRUE);
					

				break;


				case ID_RED:

					TextToSpeech = L"Changing wave color to Red.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					// change the color to red
					currentColor = RGB(255, 0, 0);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Red.\n");
					
				break;

				case ID_GREEN:
					
					TextToSpeech = L"Changing wave color to Green.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to green
					currentColor = RGB(0, 255, 0);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
					
					if(file)
						fprintf(file, "User Changed wave color to Green.\n");
					
				break;

				case ID_BLUE:
					
					TextToSpeech = L"Changing wave color to Blue.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to red
					currentColor = RGB(0, 0, 255);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Blue.\n");
					
				break;

				case ID_CYAN:
					
					TextToSpeech = L"Changing wave color to Cyan.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to cyan
					currentColor = RGB(0, 255, 255);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Cyan.\n");
					
				break;

				case ID_MAGENTA:
					
					TextToSpeech = L"Changing wave color to Magenta.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to magenta
					currentColor = RGB(255, 0, 255);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
					
					if(file)
						fprintf(file, "User Changed wave color to Magenta.\n");
					
				break;

				case ID_YELLOW:
					
					TextToSpeech = L"Changing wave color to Yellow.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					// change the color to yellow
					currentColor = RGB(255, 255, 0);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Yellow.\n");
					
				break;

				case ID_WHITE:
					
					TextToSpeech = L"Changing wave color to White.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to white
					currentColor = RGB(255, 255, 255);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to White.\n");
					
				break;

				case ID_GRAY:
					
					TextToSpeech = L"Changing wave color to Gray.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					// change the color to gray
					currentColor = RGB(128, 128, 128);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Gray.\n");
					
				break;
			
				case ID_VIOLET:
					
					TextToSpeech = L"Changing wave color to Violet.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
					// change the color to violet
					currentColor = RGB(238, 130, 238);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Violet.\n");
					
				break;

				case ID_ORANGE:
					
					TextToSpeech = L"Changing wave color to Orange.";
					
					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
				
					
					// change the color to orange
					currentColor = RGB(255, 128, 0);

					// Trigger Repaint
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(file)
						fprintf(file, "User Changed wave color to Orange.\n");
					
				break;

				case ID_INC_PTS:
				
					if(num_points < 3000)
					{
						TextToSpeech = L"Increasing number of points.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						num_points += 100;

						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
					
						if(file)
							fprintf(file, "User Increased number of points to %d .\n", num_points);
					
					}
					
				break;
			
				case ID_DEC_PTS:
					
					if(num_points > 500)
					{
						TextToSpeech = L"Decreasing number of points.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						num_points -= 100;

						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
						
						if(file)
							fprintf(file, "User Decreased number of points to %d .\n", num_points);
					
					}

				
				break;
			
				case ID_INC_WAV:
					
					if(WAVE_COUNT < 5)
					{
						WAVE_COUNT += 1;
						
						TextToSpeech = L"Increasing number of waves.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);

						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
						
						if(file)
							fprintf(file, "User Increased number of waves to %d .\n", WAVE_COUNT);
					}

		

				break;
			
				case ID_DEC_WAV:

					if(WAVE_COUNT > 1)
					{
						WAVE_COUNT -= 1;
						
						TextToSpeech = L"Decreasing number of waves.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
						
						if(file)
							fprintf(file, "User Decreased number of waves to %d .\n", WAVE_COUNT);
					
					}
					
				
				break;
			


				case ID_INC_AMP:
					
					if(dAmplitude < 300)
					{
						TextToSpeech = L"Increasing dAmplitude of wave.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						dAmplitude += 10.0;
					
						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
						
						if(file)
							fprintf(file, "User Increased amplitude to %.2f .\n", dAmplitude);
					
					}
				
				break;
			
				case ID_DEC_AMP:
					
					if(dAmplitude > 50)
					{
						TextToSpeech = L"Decreasing dAmplitude of wave.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					
						dAmplitude -= 10.0;
					
						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
					
						if(file)
							fprintf(file, "User Decreased amplitude to %.2f .\n", dAmplitude);
					
					}
				
				break;
			
				case ID_INC_FRQ:
				
					if(dFrequency > 2)
					{
						TextToSpeech = L"Increasing Frequency of wave.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						dFrequency -= 1;
				
						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

						if(file)
							fprintf(file, "User Increased frequency to %.2f .\n", dFrequency);
					}
				
				break;
			
				case ID_DEC_FRQ:

					if(dFrequency < 20)
					{
						TextToSpeech = L"Decreasing Frequency of wave.";
					
						if(bSoundOn)
							pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
						dFrequency += 1;

						InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
						
						if(file)
							fprintf(file, "User Decreased frequency to %.2f .\n", dFrequency);
					}
				
				break;
			
				case ID_RST_WAV:
					
					for(i = 0; i < NUM_POINTS; i++)
					{	
						// Animating Multiple Points
						iPlotPoint[i].x 		= 0;
						iPlotPoint[i].y 		= 0;
						iFinalPlotPoint[i].x 	= 0;
						iFinalPlotPoint[i].y 	= 0;

					}

					for(i = 0; i < NUM_POINTS; i++)
					{
						iPlotPoint[i].x = -(i * POINT_SIZE);
					}
					
					TextToSpeech = L"Resetting wave.";
					
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);

					if(bSoundOn)
						pISpVoice -> Speak(TextToSpeech, SPF_ASYNC, NULL);
					
					if(file)
						fprintf(file, "User reset the wave.\n");
					
				break;

				case ID_ABT:
					
					// Call GetWindowLongPtr()
					hInst = ((HINSTANCE) (GetWindowLongPtr(hWndDlg, GWLP_HINSTANCE)));
					
					// Call DialogBox
					DialogBox(hInst, TEXT("ABOUT"), hWndDlg, ((DLGPROC) (AboutDlgProc)));
				
					InvalidateRect(GetParent(hWndDlg), NULL, TRUE);
					
					if(file)
						fprintf(file, "User clicked on about button.\n");
					
				break;

				// Vol on Radio Button
				case ID_VOL_ON:
				
					// To display it is checked
					SendDlgItemMessage(hWndDlg, ID_VOL_ON, BM_SETCHECK, 1, 0);
					bSoundOn = TRUE;

					if(file)
						fprintf(file, "User clicked on volume on radio button.\n");


				break;
				
				// Vol off Radio Button
				case ID_VOL_OFF:
					
					// To display it is checked
					SendDlgItemMessage(hWndDlg, ID_VOL_OFF, BM_SETCHECK, 1, 0);
					bSoundOn = FALSE;
					
					if(file)
						fprintf(file, "User clicked on volume off radio button.\n");


				break;
				
				// Export Audio Check Box
				case IDCB_EXPAUDCB:
					
					// To display it is checked
					//SendMessage (hWndDlg, BM_SETCHECK, 1, 0);

					// TRUE - button is checked
					// FALSE - button is not checked
					iCheckAudioLog = (int)SendMessage (hWndDlg, BM_GETCHECK, 0, 0);
					bCheckBoxAudioLog = TRUE;

					//wsprintf(str, TEXT("iCheckAudioLog = %i"), iCheckAudioLog);
					//MessageBox(hWndDlg, str, TEXT("Debug"), MB_OK);

					if(iCheckAudioLog != 0)
						bCheckBoxAudioLog = TRUE;
				
					// Audio log will be exported in WM_DESTROY
					
					
				break;

				case ID_LISTBOX:

					switch(HIWORD(wParam))
					{
							case LBN_SELCHANGE:

								hList = GetDlgItem(hWndDlg, ID_LISTBOX);

								// Get index of current selected string
								index = (int)SendMessage(hList, LB_GETCURSEL, 0, 0);
	
								pISpVoice -> SetVoice(pISpObjectToken);


							break;
					}

				break;
				
				default:
		
				break;


			}
		
			break;
	}
	
	return FALSE;
}

// Dialog Box Procedure
BOOL CALLBACK AboutDlgProc(HWND hWndAboutDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HICON hIcon;
	HDC hdcStatic;	
	static HBRUSH hbr = NULL;
	
	
	switch(iMsg)
	{
		case WM_INITDIALOG:
			
			
	
		return TRUE;
		
		case WM_CTLCOLORDLG:

		// Set the background color of dialog box - black
		return ((INT_PTR)CreateSolidBrush(ABT_DLG_BOX_CLR));

		case WM_CTLCOLORSTATIC:
			
			hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(0,255,255));	// light blue
			SetBkColor(hdcStatic, RGB(0,0,0));
			
			if (hbr == NULL)
				hbr = CreateSolidBrush(RGB(0,0,0));
        
		
		return (INT_PTR)hbr;
        
		
		case WM_COMMAND:

			switch(LOWORD(lParam))
			{
				case IDOK:
					// 2nd parameter is used as return value of Dialog Box
					EndDialog(hWndAboutDlg, 0);
					
				return TRUE;
				
				
				case IDCANCEL:
				
					// 2nd parameter is used as return value of Dialog Box
					EndDialog(hWndAboutDlg, 0);
					
				return TRUE;
				

			}
			
		return TRUE;
		
		case WM_CLOSE:
			
			EndDialog(hWndAboutDlg, 0);
			
		return TRUE;


		default:
		
		break;


	}
	return(FALSE);
}

// Dialog Box Procedure
BOOL CALLBACK RegisterDlgProc(HWND hWndRegisterDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// variable declarations
	HDC hdc;
	char firstName[50];
	char surname[50];
	int  countFirstName = 0;
	int  countSurname = 0;
	static BOOL bCatchFirstNameError 	= false;
	static BOOL bCatchSurnameError 	= false;
	hWndDlg = GetParent(hWndRegisterDlg);
	TCHAR str[255];
	int iFirstNameSpotValidationCount = -1;
	int iSurnameSpotValidationCount = -1;
	
	switch(iMsg)
	{
		case WM_INITDIALOG:
			
			EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), TRUE);
			
			// Set focus in name edit box
			SetFocus(GetDlgItem(hWndRegisterDlg, ID_FIRST_NAME));
		
			bCatchFirstNameError   = false;
			bCatchSurnameError 	 = false; 


			
				

		break;
		
		case WM_CTLCOLORDLG:

		// Set the background color of dialog box - pink
		return ((INT_PTR)CreateSolidBrush(RGT_DLG_BOX_CLR));

		

		case WM_CTLCOLORSTATIC:
			
			hdc = (HDC)wParam;
			SetBkColor(hdc, RGT_DLG_BOX_CLR);
			
			// lParam is handle of static text


			if (bCatchFirstNameError || !(bCatchSurnameError))
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				
			}

			else
			{
				SetTextColor(hdc, RGB(0, 0, 0));
				
			}

			if (bCatchSurnameError)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				
			}

			else
			{
				SetTextColor(hdc, RGB(0, 0, 0));
				
			}

			

		return ((INT_PTR)CreateSolidBrush(RGT_DLG_BOX_CLR));
		


		case WM_COMMAND:

			
			// Spot Validation
			switch(LOWORD(wParam))
			{
				
				case IDOK:
					// 2nd parameter is used as return value of Dialog Box
					EndDialog(hWndRegisterDlg, 0);
					
				break;
				
				case IDCANCEL:
					
					// 2nd parameter is used as return value of Dialog Box
					EndDialog(hWndRegisterDlg, IDCANCEL);
					
				break;
				
				// Spot Validation
				case ID_REGISTER:
				
					if(bCatchFirstNameError || bCatchSurnameError)
						bUserRegistered = false;
					
					
					GetLocalTime(&SysTime);
					
					GetDlgItemText(hWndRegisterDlg, ID_ETNAME, firstName, sizeof(firstName));
					GetDlgItemText(hWndRegisterDlg, ID_ETSURNAME, surname, sizeof(surname));

					sprintf(FileName, "%s%s-%02d-%02d-%d-%02d-%02d-%02d.txt", firstName, surname, SysTime.wDay, SysTime.wMonth, SysTime.wYear, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
					sprintf(FileNameForAudio, "%s-%s-%02d-%02d-%d-%02d-%02d-%02d.wav", firstName, surname, SysTime.wDay, SysTime.wMonth, SysTime.wYear, SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
					
					
					file = fopen(FileName, "w");    
					
					if(file == NULL)
					{
						MessageBox(NULL, TEXT("File Open Failed"), TEXT("Error"), MB_OK | MB_ICONERROR);
						exit(0);
					}
					
					
					fprintf(file, "Registered User Name: %s %s \n", firstName, surname);
					fprintf(file, "Registered Time: %02d:%02d:%02d \n", SysTime.wHour, SysTime.wMinute, SysTime.wSecond);
					
					bUserRegistered = TRUE;
					
					EndDialog(hWndRegisterDlg, 0);
				
				break;
				
				
				
			}	

			
			switch(HIWORD(wParam))
			{
				case EN_KILLFOCUS:
      
					GetDlgItemText(hWndRegisterDlg, ID_ETNAME, firstName, sizeof(firstName));

					countFirstName = 0;

					for (int i = 0; firstName[i] != '\0'; i++)
					{
						countFirstName++;
					}

					
					if (countFirstName <= 0)
					{
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), FALSE);
					}


					for (int i = 0; i < countFirstName; i++)
					{
						
						// if firstName contains nos. or special characters
						if (!((firstName[i] >= 'a' && firstName[i] <= 'z') || (firstName[i] >= 'A' && firstName[i] <= 'Z')))
						{
							bCatchFirstNameError = true; // error

							iFirstNameSpotValidationCount++;

						}
					}

					
					
					if (iFirstNameSpotValidationCount > 0)
					{
						bCatchFirstNameError = true;
						SendDlgItemMessage(hWndRegisterDlg, ID_FIRST_NAME, WM_ENABLE, wParam, 0);
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), FALSE);
					}

					else
					{
						bCatchFirstNameError = false;
						SendDlgItemMessage(hWndRegisterDlg, ID_FIRST_NAME, WM_ENABLE, wParam, 0);
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), TRUE);
					}

					GetDlgItemText(hWndRegisterDlg, ID_ETSURNAME, surname, sizeof(surname));

					for (int i = 0; surname[i] != '\0'; i++)
					{
						countSurname++;
					}

					if (countSurname <= 0)
					{
						
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), FALSE);
						
						
					}

					

					for (int i = 0; i < countSurname; i++)
					{
						if (!((surname[i] >= 'a' && surname[i] <= 'z') || (surname[i] >= 'A' && surname[i] <= 'Z')))
						{
							bCatchSurnameError = true;
							
							iSurnameSpotValidationCount ++;

						}
					}

					
					if (iSurnameSpotValidationCount > 0)
					{
						bCatchSurnameError = true;
						SendDlgItemMessage(hWndRegisterDlg, ID_SURNAME, WM_ENABLE, wParam, 0);
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), FALSE);
					}

					else
					{
						bCatchSurnameError = false;
						SendDlgItemMessage(hWndRegisterDlg, ID_SURNAME, WM_ENABLE, wParam, 0);
						EnableWindow(GetDlgItem(hWndRegisterDlg, ID_REGISTER), TRUE);
					}

				break;


			
			}
			

		return TRUE;

		case WM_CLOSE:
			
			EndDialog(hWndRegisterDlg, 0);
			
		break;

		default:
		
		break;

	}
	return(FALSE);


}


void SafeInterfaceRelease()
{
	if(pICircle)
	{
		pICircle -> Release();
		pICircle = NULL;
	}
	
	if(pISineWave)
	{
		pISineWave -> Release();
		pISineWave = NULL;
	}

	if(pISpStream)
	{
		pISpStream -> Release();
		pISpStream = NULL;
		
	}
	
	
	if(pISpObjectToken)
	{
		pISpObjectToken -> Release();
		pISpObjectToken = NULL;
		
	}


	if(pIEnumSpObjectTokens)
	{
		pIEnumSpObjectTokens -> Release();
		pIEnumSpObjectTokens = NULL;
		
	}
	
	if(pISpVoice)
	{
		pISpVoice -> Release();
		pISpVoice = NULL;
	}
	
	if(file)
	{
		fclose(file);
		file = NULL;
	}
	
	
}


















