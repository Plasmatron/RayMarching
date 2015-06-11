/*
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"ws2_32.lib")



#include "main.h"
#include "renderer.h"

#define BUFFER_OFFSET(i) ((void*)(i))
#define RIGHT_HAND 1
#define LEFT_HAND 0

HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;






void Init(HWND hWnd)
{		
	



	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);
	glewInit();	
	glDisable(GL_CULL_FACE);	
	glClearColor ( 0.0, 0.5, 0.7, 0.0 ) ;
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	
	
}

WPARAM MainLoop()
{	
		
	MSG msg;
	const char* strError = "win";
	Renderer *renderer = new Renderer();
	
	float wait = 0;


	



	while(1)
	{	
				
				
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
			if(msg.message == WM_QUIT)
				break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		else
		{			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//renderer->RenderScene(test);		
			renderer->RenderScene();
			SwapBuffers(g_hDC);
				
        }
	}
			

	
	DeInit();
 
	return(msg.wParam);
}
 

LRESULT CALLBACK WinProc(HWND hWnd,UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
    PAINTSTRUCT    ps;
 
    switch (uMsg)
	{
    case WM_SIZE:		// Если изменён размер окна
 
		SizeOpenGLScreen(LOWORD(lParam),HIWORD(lParam));
		GetClientRect(hWnd, &g_rRect);		
        break;
 
	case WM_PAINT:		
		BeginPaint(hWnd, &ps);	
		EndPaint(hWnd, &ps);	
		break;
 
	case WM_KEYDOWN:	
				
		switch(wParam)
		{
			case VK_ESCAPE:		
				PostQuitMessage(0);	
				break;
			case 0x42:
				break;

		}
		break;
 
    case WM_CLOSE:		
        PostQuitMessage(0);	
        break;
 
    default:		
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam);
        break;
    //case WM_LBUTTONDOWN:                   


    }

 
    return lRet;
}


*/