#pragma once
#include "Drawable.h"

class Triangle : public Drawable
{
public:
	Triangle( Graphics& gfx) noexcept;
    void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransform() const noexcept override;

private:
    float angle;
};