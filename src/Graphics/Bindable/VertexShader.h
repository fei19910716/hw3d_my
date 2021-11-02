#pragma once
#include "Bindable.h"

class VertexShader: public Bindable{
public:
    VertexShader(Graphics& gfx, const LPCWSTR& path) noexcept;
    void Bind(Graphics& gfx) noexcept override;
    ID3DBlob* GetByteCode() const noexcept;

protected:
    Microsoft::WRL::ComPtr<ID3DBlob> pByteCodeBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
};