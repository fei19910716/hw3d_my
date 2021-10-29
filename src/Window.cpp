#include "Window.h"
#include "WindowsMessageMap.h"
#include <sstream>


Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
    :hInst(GetModuleHandle(nullptr))
{
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = HandleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetInstance();
    wc.hIcon = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszClassName = GetName();
    wc.hIconSm = nullptr;
    RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName,GetInstance());
}

const TCHAR* Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

//
// Window
//
Window::Window(int width, int height, const TCHAR* name) noexcept{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    AdjustWindowRect(&wr,WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,FALSE);

    hWnd = CreateWindow(
        WindowClass::GetName(),name,
        WS_CAPTION|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
        CW_USEDEFAULT,CW_USEDEFAULT,wr.right-wr.left,wr.bottom-wr.top,
        nullptr,nullptr,WindowClass::GetInstance(),this);

    ShowWindow(hWnd,SW_SHOWDEFAULT);
}

Window::~Window(){
    DestroyWindow(hWnd);
}

LRESULT CALLBACK Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
    if(msg == WM_NCCREATE){
        const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&Window::HandleMsgTrunk));
        return pWnd->HandleMsg(hWnd,msg,wParam,lParam);
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}

LRESULT CALLBACK Window::HandleMsgTrunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd,GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd,msg,wParam,lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept{
    static WindowsMessageMap mmp;

    OutputDebugStringA(mmp(msg,lParam,wParam).c_str() );
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if(wParam == 'D'){
            SetWindowText(hWnd,TEXT("Respects"));
        }
        break;

    case WM_KEYUP:
        if(wParam == 'F'){
            SetWindowText(hWnd,TEXT("Danger"));
        }
        break;

    case WM_CHAR:
        {
            static std::string title;
            title.push_back((char)wParam);
            SetWindowTextA(hWnd,title.c_str());
        }
        break;

    case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            std::ostringstream oss;
            oss << "("<<pt.x << "," << pt.y << ")";
            SetWindowTextA(hWnd,oss.str().c_str() );
        }
        break;
    
    default:
        break;
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}