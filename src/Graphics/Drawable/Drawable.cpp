#include "Drawable.h"
#include "Graphics/GraphicsThrowMacros.h"
#include "Graphics/Bindable/IndexBuffer.h"
#include <cassert>
#include <typeinfo>

void Drawable::Draw(Graphics& gfx) const noexcept{
    for(auto& b: binds){
        b->Bind(gfx);
    }

    gfx.DrawIndexed(indiceCount);
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept{
    if(typeid(*bind) == typeid(IndexBuffer)){
        auto naked_ptr = dynamic_cast<IndexBuffer*>(bind.get());
        indiceCount = naked_ptr->GetCount();
    } 
    binds.push_back( std::move( bind ) );
}