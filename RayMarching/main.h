#ifndef _MAIN_H
#define _MAIN_H


#include <gl/glew.h>
#include <gl/wglew.h>


#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/glu.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_DEPTH 32



extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;
extern HMODULE hModule;
//typedef unsigned int uint;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WPARAM MainLoop();
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);
bool bSetupPixelFormat(HDC hdc);
void SizeOpenGLScreen(int width, int height);
void InitializeOpenGL(int width, int height);
void Init(HWND hWnd);
void DeInit();

#endif