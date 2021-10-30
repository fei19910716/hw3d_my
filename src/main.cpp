#include "App.h"
#include <sstream>
int CALLBACK WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrev,
    LPSTR lpCmdLine,
    int nCmdShow
){
    try
    {
        return App{}.Go();
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