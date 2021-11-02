#pragma once
#include "Drawable.h"
#include "Graphics/Bindable/IndexBuffer.h"

template<class T>
class DrawableBase : public Drawable
{
public:
	bool IsStaticInitialized() const noexcept
	{
		return !staticBinds.empty();
	}
	void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept
	{
		if(typeid(*bind) == typeid(IndexBuffer)){
			auto naked_ptr = dynamic_cast<IndexBuffer*>(bind.get());
			indiceCount = naked_ptr->GetCount();
		}
    
		staticBinds.push_back( std::move( bind ) );
	}
private:
	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
	{
		return staticBinds;
	}	
	const UINT GetIndiceCount() const noexcept override
	{
		return indiceCount;
	}	
private:
	static UINT indiceCount;
	static std::vector<std::unique_ptr<Bindable>> staticBinds;
};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;

template<class T>
UINT DrawableBase<T>::indiceCount;