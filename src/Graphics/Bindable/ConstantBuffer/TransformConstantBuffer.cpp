#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) noexcept
    :parent(parent)    
{
    if(!vcbuf){
        vcbuf = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
    }
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

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformConstantBuffer::vcbuf;