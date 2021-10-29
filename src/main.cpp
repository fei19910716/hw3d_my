#include "Window.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrev,
    LPSTR lpCmdLine,
    int nCmdShow
){
   Window wnd(640,480,TEXT("Donky Fart Box"));

    MSG msg;
    BOOL gResult;    
    while (gResult = GetMessage(&msg,nullptr,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if(gResult == -1){
        return -1;
    }
    return msg.wParam;
}