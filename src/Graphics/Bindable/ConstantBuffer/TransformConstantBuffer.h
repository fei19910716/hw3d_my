#pragma once
#include "ConstantBuffer.h"
#include "Graphics/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer: public Bindable{
public:
    TransformConstantBuffer(Graphics& gfx, const Drawable& parent) noexcept;
    void Bind(Graphics& gfx) noexcept override;

private:
    std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> vcbuf;
    const Drawable& parent;
};