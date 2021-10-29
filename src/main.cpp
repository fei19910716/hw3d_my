#define NOUNICODE
#include <iostream>
#include <windows.h>
#include "WindowsMessageMap.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg, WPARAM wParam, LPARAM lParam){
    static WindowsMessageMap mmp;

    OutputDebugStringA(mmp(msg,lParam,wParam).c_str() );
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    
    default:
        break;
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrev,
    LPSTR lpCmdLine,
    int nCmdShow
){
    const auto pClassName = TEXT("hw3d_my");
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = pClassName;
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindowEx(
        0,pClassName,
        TEXT("happy hard window"),
        WS_CAPTION|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
        200,200,640,480,
        nullptr,nullptr,hInstance,nullptr);

    ShowWindow(hWnd,SW_SHOW);

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