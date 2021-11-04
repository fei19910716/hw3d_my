#pragma once
#include "Graphics/Drawable/DrawableBase.h"

class SolidSphere: public DrawableBase<SolidSphere>{
public:
    SolidSphere(Graphics& gfx,float radius);
    void Update(float dt) noexcept override;
    void SetPos(DirectX::XMFLOAT3 pos) noexcept;
    DirectX::XMMATRIX GetTransform() const noexcept override;
private:
    DirectX::XMFLOAT3 pos = {1.0f,1.0f,1.0f};
};