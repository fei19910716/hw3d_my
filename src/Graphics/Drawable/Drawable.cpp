#include "Drawable.h"
#include "Graphics/GraphicsThrowMacros.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept{
    for(auto& b: binds){
        b->Bind(gfx);
    }

    for(auto& b : GetStaticBinds()){
        b->Bind(gfx);
    }

    gfx.DrawIndexed(GetIndiceCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept{
    binds.push_back( std::move( bind ) );
}