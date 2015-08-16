#if WIN32

#include <GLES2\gl2.h>
#include <windows.h>
#include <EGL\egl.h>
#include <cstdio>
#include "Timer.h"
#include "ESContext.h"

extern ESContext context_tt;

EGLDisplay			eglDisplay;
EGLConfig			eglConfig;
EGLSurface			eglSurface;
EGLContext			eglContext;
EGLNativeWindowType	eglWindow;

// Windows variables
HWND hWnd;
HDC	 hDC;
void CleanWin();
bool TestEGLError(HWND hWnd, char* pszLocation);
bool InitWindow(int width, int height);

extern void fireLoopEvent();
extern void fireInitEvent();
extern void fireDrawEvent();
extern void fireUpdateEvent();
extern void fireDestroyEvent();
extern void fireMouseEvent(int type, int x, int y);
extern void fireKeyEvent(int key_code, bool is_down);
extern void fireTickEvent();

//Process Input
LRESULT CALLBACK WndProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT  lRet = 1; 
	int x = LOWORD(lParam); 
	int y = HIWORD(lParam);
	static bool mouseDown = false;

	switch (Msg) 
	{ 
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(WM_QUIT);
		break; 

	case WM_KEYDOWN:
		fireKeyEvent((int) wParam, true);
		break;
	case WM_KEYUP:
		fireKeyEvent((int) wParam, false);
		break;
	case WM_LBUTTONDOWN:		
	case WM_RBUTTONDOWN:		
		mouseDown = true;		
		fireMouseEvent(0, x, y);
		break;

	case WM_LBUTTONUP:	
	case WM_RBUTTONUP:
		mouseDown = false;
		fireMouseEvent(1, x, y);
		break;

	case WM_MOUSEMOVE:		
		if (mouseDown)
		{			
			fireMouseEvent(2, x, y);
		}		
		break;
	default: 
		lRet = DefWindowProc (hWnd, Msg, wParam, lParam); 
		break; 
	} 

	return lRet; 
}

void CleanWin()
{
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

	eglTerminate(eglDisplay);

	if (hDC) 
		ReleaseDC(hWnd, hDC);

	if (hWnd) 
		DestroyWindow(hWnd);
}

bool InitWindow(int width, int height)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	// Register the windows class
	WNDCLASS sWC;
	sWC.style = CS_HREDRAW | CS_VREDRAW;
	sWC.lpfnWndProc = WndProcedure; //Procedute Callback Event
	sWC.cbClsExtra = 0;
	sWC.cbWndExtra = 0;
	sWC.hInstance = hInstance;
	sWC.hIcon = 0;
	sWC.hCursor = 0;
	sWC.lpszMenuName = 0;
	sWC.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	sWC.lpszClassName = "OpenGLES2.0";


	ATOM registerClass = RegisterClass(&sWC);
	if (!registerClass)
	{
		MessageBox(0, ("Failed to register the window class"), ("Error"), MB_OK | MB_ICONEXCLAMATION);
	}
	// Create the eglWindow
	RECT	sRect;
	SetRect(&sRect, 0, 0, width, height);
	AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
	hWnd = CreateWindow( "OpenGLES2.0", "noem", WS_VISIBLE | WS_SYSMENU,
		0, 0, sRect.right - sRect.left, sRect.bottom - sRect.top, NULL, NULL, hInstance, NULL);
	eglWindow = hWnd;

	// Get the associated device context
	hDC = GetDC(hWnd);
	if (!hDC)
	{
		MessageBox(0, "Failed to create the device context", "Error", MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	eglDisplay = eglGetDisplay(hDC);

	if(eglDisplay == EGL_NO_DISPLAY)
		eglDisplay = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);

	EGLint iMajorVersion, iMinorVersion;
	if (!eglInitialize(eglDisplay, &iMajorVersion, &iMinorVersion))
	{
		MessageBox(0, ("eglInitialize() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}

	const EGLint pi32ConfigAttribs[] =
	{
		EGL_LEVEL,				0,
		EGL_SURFACE_TYPE,		EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE,	EGL_OPENGL_ES2_BIT,
		EGL_NATIVE_RENDERABLE,	EGL_FALSE,		
		EGL_DEPTH_SIZE,			8,
		EGL_STENCIL_SIZE, 8,
		EGL_SAMPLE_BUFFERS, 1,
		EGL_NONE
	};


	int iConfigs;
	if (!eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1, &iConfigs) || (iConfigs != 1))
	{
		MessageBox(0, ("eglChooseConfig() failed."), ("Error"), MB_OK|MB_ICONEXCLAMATION);
		CleanWin();
		return false;
	}


	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, eglWindow, NULL);

	if(eglSurface == EGL_NO_SURFACE)
	{
		eglGetError(); // Clear error
		eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, NULL, NULL);
	}

	if (!TestEGLError(hWnd, "eglCreateWindowSurface"))
	{
		CleanWin();
		return false;
	}


	// Bind the API (It could be OpenGLES or OpenVG)
	eglBindAPI(EGL_OPENGL_ES_API);
	EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
	eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, ai32ContextAttribs);

	if (!TestEGLError(hWnd, "eglCreateContext"))
	{
		CleanWin();
		return false;
	}


	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

	if (!TestEGLError(hWnd, "eglMakeCurrent"))
	{
		CleanWin();
		return false;
	}

	return true;
}
bool TestEGLError(HWND hWnd, char* pszLocation)
{	
	EGLint iErr = eglGetError();
	if (iErr != EGL_SUCCESS)
	{
		TCHAR pszStr[256];
		sprintf(pszStr, ("%s failed (%d).\n"), pszLocation, iErr);
		MessageBox(hWnd, pszStr, ("Error"), MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	return true;
}

void WinLoop()
{	
	MSG msg = {0};
	bool done = false;
	fireInitEvent();
	while (!done)
	{
		fireTickEvent();
		if  (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				done = true; 
			}
			else
			{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
		}
		else
		{
			
		}

		fireDrawEvent();
		fireUpdateEvent();

		eglSwapBuffers(eglDisplay, eglSurface);	
	}

	fireDestroyEvent();
}

#endif