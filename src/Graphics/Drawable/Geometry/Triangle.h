#pragma once
#include "Graphics/Drawable/DrawableBase.h"

class Triangle : public DrawableBase<Triangle>
{
public:
	Triangle( Graphics& gfx) noexcept;
    void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
    float angle;
};