#pragma once
#include "ConstantBuffer.h"
#include "Graphics/Drawable/Drawable.h"
#include <DirectXMath.h>

class TransformConstantBuffer: public Bindable{
private:
    // Vertex shader 中的constant buffer
    struct Transforms{
        DirectX::XMMATRIX modelViewProj;
        DirectX::XMMATRIX model;
    };
public:
    TransformConstantBuffer(Graphics& gfx, const Drawable& parent) noexcept;
    void Bind(Graphics& gfx) noexcept override;

private:
    static std::unique_ptr<VertexConstantBuffer<Transforms>> vcbuf;
    const Drawable& parent;
};