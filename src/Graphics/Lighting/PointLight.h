#pragma once
#include "Graphics/Graphics.h"
#include "Graphics/Drawable/Geometry/SolidSphere.h"
#include "Graphics/Bindable/ConstantBuffer/ConstantBuffer.h"

class PointLight{
public:
    PointLight(Graphics& gfx,float radius = 0.5f);
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;
    void Draw(Graphics& gfx) const noexcept;
    void Bind(Graphics& gfx) const noexcept;
private:
    struct PointLightCBuf
    {
        DirectX::XMFLOAT3 pos;
        float padding;
    };

private:
    DirectX::XMFLOAT3 pos = {0.0f,0.0f,0.0f};
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};