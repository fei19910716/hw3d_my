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
    void Bind(Graphics& gfx, DirectX::FXMMATRIX view) const noexcept;
private:
    struct PointLightCBuf
    {
        // GPU 需要四字节对齐，否则会crash
        alignas(16) DirectX::XMFLOAT3 pos;

        alignas(16) DirectX::XMFLOAT3 ambient;
        alignas(16) DirectX::XMFLOAT3 diffuseColor;
        float diffuseIntensity;
        float attConst;
        float attLin;
        float attQuad;
    };

private:
    PointLightCBuf cbData;
    mutable SolidSphere mesh;
    mutable PixelConstantBuffer<PointLightCBuf> cbuf;
};