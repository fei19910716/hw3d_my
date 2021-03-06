#pragma once
#include "FordException.h"
#include "Debug/DXGIDebugInfoManager.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
class Graphics{
    friend class Bindable;
public:
    class Exception: public FordException{
        using FordException::FordException;
    };
    class HrException : public Exception{
    public:
        HrException(int line, const char* file, HRESULT hr,std::vector<std::string> infoMsgs = {}) noexcept;
        const char* what() const noexcept override;
        const char* GetType() const noexcept override;
        HRESULT GetErrorCode() const noexcept;
        std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
        std::string GetErrorInfo() const noexcept;
    private:
        HRESULT hr;
        std::string info;
    };

    class InfoException: public Exception{
    public:
        InfoException(int line,const char* file,std::vector<std::string> infoMsgs) noexcept;
        const char* what() const noexcept override;
		const char* GetType() const noexcept override;
        std::string GetErrorInfo() const noexcept;
    private:
        std::string info;
    };

    class DeviceRemovedException: public HrException{
        using HrException::HrException;
    public:
        const char* GetType() const noexcept override;
    private:
		std::string reason;
    };

public:
    Graphics(HWND hWnd);
    Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
    ~Graphics();
    void EndFrame();
    void BeginFrame(float red, float green, float blue) noexcept;

    void DrawIndexed(UINT count) noexcept;
    void SetProjection(DirectX::XMMATRIX proj) noexcept;
    DirectX::XMMATRIX GetProjection() const noexcept;

    void SetCamera( DirectX::FXMMATRIX cam ) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

    void EnableImGui() noexcept;
    void DisableImGui() noexcept;
    bool IsImGuiEnabled() const noexcept;
private:
    bool imGuiEnabled = true;
    DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
    Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
    Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

    #ifndef NDEBUG
	DXGIDebugInfoManager infoManager; // debug???????????????????????????debug??????
    #endif
};