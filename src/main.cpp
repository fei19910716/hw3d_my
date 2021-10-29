#include "Window.h"

int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrev,
    LPSTR lpCmdLine,
    int nCmdShow
){
    try
    {
        Window wnd(640,480,TEXT("Donky Fart Box"));

        MSG msg;
        BOOL gResult;    
        while (gResult = GetMessage(&msg,nullptr,0,0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if(wnd.kbd.KeyIsPressed(VK_SPACE)){
                MessageBoxA(nullptr,"Something Happen","Standard Exception",MB_OK|MB_ICONEXCLAMATION);
            }
        }
        if(gResult == -1){
            throw LAST_EXCEPTION();
        }
        return msg.wParam;
    }
    catch(const FordException& e){
        MessageBoxA(nullptr,e.what(),e.GetType(),MB_OK|MB_ICONEXCLAMATION);
    }
    catch(const std::exception& e)
    {
        MessageBoxA(nullptr,e.what(),"Standard Exception",MB_OK|MB_ICONEXCLAMATION);
    }
    catch(...){
        MessageBoxA(nullptr,"No Details Available","Standard Exception",MB_OK|MB_ICONEXCLAMATION);
    }
    return -1;

}