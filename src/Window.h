#pragma once

#include <Windows.h>
#include "FordException.h"
#include "Keyboard.h"
class Window{
public:
    class Exception : public FordException{
    public:
        Exception(int line, const char* file, HRESULT hr) noexcept;
        const char* what() const noexcept override;
        virtual const char* GetType() const noexcept;

        static std::string TranslateErrorCode(HRESULT hr) noexcept;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;

    private:
        HRESULT hr;
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

private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgTrunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

public:
    Keyboard kbd;
private:
    int width;
    int height;
    HWND hWnd;
};


#define EXCEPTION(hr) Window::Execption(__LINE__,__FILE__,hr)
#define LAST_EXCEPTION() Window::Exception(__LINE__,__FILE__,GetLastError())