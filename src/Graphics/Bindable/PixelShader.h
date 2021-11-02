#pragma once
#include "Bindable.h"

class PixelShader: public Bindable{
public:
    PixelShader(Graphics& gfx, const LPCWSTR& path) noexcept;
    void Bind(Graphics& gfx) noexcept override;
    ID3DBlob* GetByteCode() const noexcept;
protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
};