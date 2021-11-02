#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) noexcept
    :parent(parent)    
{
    vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
}

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept{
    vcbuf->Update(
        gfx,
        DirectX::XMMatrixTranspose(
            parent.GetTransform() * gfx.GetProjection()
        )
    );

    vcbuf->Bind(gfx);
}