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
Window::Window(int width, int height, const TCHAR* name)
    :width(width),height(height)
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if(0 == (AdjustWindowRect(&wr,WS_CAPTION|WS_MINIMIZEBOX|WS_SYSMENU,FALSE) ) ){
        throw LAST_EXCEPTION();
    }

    hWnd = CreateWindow(
        WindowClass::GetName(),name,
        WS_CAPTION|WS_MINIMIZEBOX|WS_MAXIMIZEBOX|WS_SYSMENU,
        CW_USEDEFAULT,CW_USEDEFAULT,wr.right-wr.left,wr.bottom-wr.top,
        nullptr,nullptr,WindowClass::GetInstance(),this);

    if(hWnd == nullptr){
        throw LAST_EXCEPTION();
    }
    ShowWindow(hWnd,SW_SHOWDEFAULT);
}

Window::~Window(){
    DestroyWindow(hWnd);
}

void Window::SetTitle( const std::string& title )
{
	if( SetWindowText( hWnd,title.c_str() ) == 0 )
	{
		throw LAST_EXCEPTION();
	}
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while( PeekMessage( &msg,nullptr,0,0,PM_REMOVE ) )
	{
		// check for quit because peekmessage does not signal this via return val
		if( msg.message == WM_QUIT )
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	// return empty optional when not quitting app
	return {};
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
    case WM_KILLFOCUS:
        kbd.ClearState();
        break;

    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        if( !(lParam & 0X40000000) || kbd.AutorepeatIsEnabled() ){
            kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

    case WM_CHAR:
        kbd.OnChar(static_cast<unsigned char>(wParam));
        break;

    case WM_MOUSEMOVE:
        POINTS pt = MAKEPOINTS(lParam);
        if(pt.x >=0 && pt.x < width && pt.y >=0 && pt.y < height){
            mouse.OnMouseMove(pt.x,pt.y);
            if(!mouse.IsInWindow()){
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }else{
            if(wParam & (MK_LBUTTON | MK_RBUTTON) ){
                // 鼠标移出window, 但是案件仍然按下状态时，capture it
                mouse.OnMouseMove( pt.x,pt.y ); 
            }else{
                ReleaseCapture();
                mouse.OnMouseLeave(); 
            }
        }
        
        break;
    case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftPressed( pt.x,pt.y );
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightPressed( pt.x,pt.y );
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnLeftReleased( pt.x,pt.y );
        // release mouse if outside of window
		if( pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height )
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS( lParam );
		mouse.OnRightReleased( pt.x,pt.y );
        // release mouse if outside of window
		if( pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height )
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}

    case WM_MOUSEWHEEL:
        {
            const POINTS pt = MAKEPOINTS(lParam);
		    const int delta = GET_WHEEL_DELTA_WPARAM( wParam );
		    mouse.OnWheelDelta( pt.x,pt.y,delta );
        }
        break;
    
    default:
        break;
    }
    return DefWindowProc(hWnd,msg,wParam,lParam);
}



//
// Exception
//
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept: FordException(line,file), hr(hr){

}
const char* Window::Exception::what() const noexcept{
    std::ostringstream oss;
    oss << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
const char* Window::Exception::GetType() const noexcept{
    return "Ford Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM| FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,hr,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
    );

    if(nMsgLen == 0){
        return "undefined error code";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}
HRESULT Window::Exception::GetErrorCode() const noexcept{
    return hr;
}
std::string Window::Exception::GetErrorString() const noexcept{
    return TranslateErrorCode(hr);
}