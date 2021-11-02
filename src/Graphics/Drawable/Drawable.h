#pragma once
#include "Graphics/Graphics.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable{
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    Drawable& operator=(const Drawable&) = delete;

    virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
    void Draw(Graphics& gfx) const noexcept;
    virtual void Update(float dt) noexcept = 0;
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;
    
    virtual ~Drawable()  = default;

private:
    UINT indiceCount;
    std::vector<std::unique_ptr<Bindable>> binds;
};