#include "main.h"
 
/*
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance)
{
	HWND hWnd;		
	WNDCLASS wndclass;	
	memset(&wndclass, 0, sizeof(WNDCLASS));		
	wndclass.style = CS_HREDRAW | CS_VREDRAW;	
	wndclass.lpfnWndProc = WinProc;			
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		
	wndclass.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);	
	wndclass.lpszClassName = "САВЕЛЬЕВ";		
	RegisterClass(&wndclass);				
 
	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN; 
 
	g_hInstance = hInstance;
 
	
	RECT rWindow;
	rWindow.left	= 0;	
	rWindow.right	= width;		
	rWindow.top	    = 0;		
	rWindow.bottom	= height;		
 
	AdjustWindowRect( &rWindow, dwStyle, false);	
 
						
	hWnd = CreateWindow("САВЕЛЬЕВ", strWindowName, dwStyle, 0, 0,
				rWindow.right  - rWindow.left, rWindow.bottom - rWindow.top,
				NULL, NULL, hInstance, NULL);

	if(!hWnd) return NULL;			
	ShowWindow(hWnd, SW_SHOWNORMAL);	
	UpdateWindow(hWnd);		
	SetFocus(hWnd);				
	ShowCursor(false);
	
	return hWnd;
}
 
 

bool bSetupPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;	// Дескриптор формата пиксела
	int pixelformat;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Устанавливаем размер структуры
	pfd.nVersion = 1;				

    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.dwLayerMask = PFD_MAIN_PLANE;		
    pfd.iPixelType = PFD_TYPE_RGBA;		
    pfd.cColorBits = SCREEN_DEPTH;		
    pfd.cDepthBits = SCREEN_DEPTH;	
    pfd.cAccumBits = 0;
    pfd.cStencilBits = 0;
 
    
    if ( (pixelformat = ChoosePixelFormat(hdc, &pfd)) == FALSE )
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }
 

    if (SetPixelFormat(hdc, pixelformat, &pfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }
 
    return TRUE;
}
 

void SizeOpenGLScreen(int width, int height)
{
	if (height==0)		
		height=1;
 

	glViewport(0,0,width,height);
      
}
 
 

 
void InitializeOpenGL(int width, int height)
{
    g_hDC = GetDC(g_hWnd);	
 
    if (!bSetupPixelFormat(g_hDC))		
        PostQuitMessage (0);			
 
    g_hRC = wglCreateContext(g_hDC);		
    wglMakeCurrent(g_hDC, g_hRC);		
    
}
 

void DeInit()
{
	if (g_hRC)
	{
		wglMakeCurrent(NULL, NULL);	
		wglDeleteContext(g_hRC);	
	}
 
	if (g_hDC)
		ReleaseDC(g_hWnd, g_hDC);	
 
	UnregisterClass("Савельев", g_hInstance);	
	PostQuitMessage (0);		
	
}
 

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow)
{
	HWND hWnd;
 
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	hWnd = CreateMyWindow("Савельев", screenWidth, screenHeight, 1, true, hInstance);
 
	// Выходим при ошибке
	if(hWnd == NULL) return TRUE;
 
	// Инициализируем OpenGL
	Init(hWnd);
 
	// Запускаем игровой цикл
	return MainLoop();
}
*/