#pragma once
#include "Graphics/Graphics.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable{
    template<typename T>
    friend class DrawableBase;
public:
    Drawable() = default;
    Drawable(const Drawable&) = delete;
    Drawable& operator=(const Drawable&) = delete;

    virtual DirectX::XMMATRIX GetTransform() const noexcept = 0;
    void Draw(Graphics& gfx) const noexcept;
    virtual void Update(float dt) noexcept
    {}
    void AddBind(std::unique_ptr<Bindable> bind) noexcept;
    
    virtual ~Drawable()  = default;

    //
    // 查询物体身上的bindable对象
    //
    template<class T>
	T* QueryBindable() noexcept
	{
		for( auto& pb : binds )
		{
			if( auto pt = dynamic_cast<T*>(pb.get()) )
			{
				return pt;
			}
		}
		return nullptr;
	}

private:
    virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;
    virtual const UINT GetIndiceCount() const noexcept = 0;
private:
    
    std::vector<std::unique_ptr<Bindable>> binds;
};