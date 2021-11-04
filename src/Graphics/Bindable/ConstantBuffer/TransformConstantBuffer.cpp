#include "TransformConstantBuffer.h"

TransformConstantBuffer::TransformConstantBuffer(Graphics& gfx, const Drawable& parent,UINT slot) noexcept
    :parent(parent)    
{
    if(!vcbuf){
        vcbuf = std::make_unique<VertexConstantBuffer<Transforms>>(gfx,slot);
    }
}

void TransformConstantBuffer::Bind(Graphics& gfx) noexcept{
    const auto modelView = parent.GetTransform() * gfx.GetCamera();
    const Transforms tf = {
        DirectX::XMMatrixTranspose(modelView),
        DirectX::XMMatrixTranspose(
            modelView * gfx.GetProjection()
        )
    };
    vcbuf->Update(gfx, tf);

    vcbuf->Bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformConstantBuffer::Transforms>> TransformConstantBuffer::vcbuf;