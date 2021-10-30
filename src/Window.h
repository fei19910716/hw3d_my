#pragma once

#include <Windows.h>
#include "FordException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics/Graphics.h"
#include <optional>
#include <memory>
class Window{
public:
    class Exception : public FordException{
        using FordException::FordException;
    public:
        static std::string TranslateErrorCode(HRESULT hr) noexcept;
    };
    class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
	private:
		HRESULT hr;
	};
	class NoGraphicsException : public Exception
	{
	public:
		using Exception::Exception;
		const char* GetType() const noexcept override;
	};
private:
    class WindowClass{
    public:
        static const TCHAR* GetName() noexcept;
        static HINSTANCE GetInstance() noexcept;

    private:
        WindowClass() noexcept;
        ~WindowClass();
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
        static constexpr const TCHAR* wndClassName = TEXT("Direct3D Engine Window");
        static WindowClass wndClass;
        HINSTANCE hInst;

    };

public:
    Window(int width, int height, const TCHAR* name);
    ~Window();
    Window(const Window& ) = delete;
    Window& operator=(const Window&) = delete;
    void SetTitle(const std::string& title);
    static std::optional<int> ProcessMessages();
    Graphics& GetGraphics();
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgTrunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard kbd;
    Mouse mouse;
private:
    int width;
    int height;
    HWND hWnd;
    std::unique_ptr<Graphics> pGraphics;
};


#define EXCEPTION(hr) Window::HrException(__LINE__,__FILE__,hr)
#define LAST_EXCEPTION() Window::HrException(__LINE__,__FILE__,GetLastError())
#define NOGRAPHICS_EXCEPTION() Window::NoGraphicsException( __LINE__,__FILE__ )
