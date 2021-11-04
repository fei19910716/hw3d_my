#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent) noexcept
    :parent(parent)    
{
    if(!vcbuf){
        vcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx);
    }
}

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept{
    const auto model = parent.GetTransform();
    const Transforms tf = {
        DirectX::XMMatrixTranspose(model),
        DirectX::XMMatrixTranspose(
            model * gfx.GetCamera() * gfx.GetProjection()
        )
    };
    vcbuf->Update(gfx, tf);

    vcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::vcbuf;