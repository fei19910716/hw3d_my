#include "Window.h"
#include <sstream>
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
			// test code
			while( !wnd.mouse.IsEmpty() )
			{
				const auto e = wnd.mouse.Read();
				switch( e.GetType() )
				{
				case Mouse::Event::Type::kLeave:
					wnd.SetTitle( "Gone!" );
					break;
				case Mouse::Event::Type::kMove:
					{
						std::ostringstream oss;
						oss << "Mouse moved to (" << e.GetPosX() << "," << e.GetPosY() << ")";
						wnd.SetTitle( oss.str() );
					}
					break;
				}
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