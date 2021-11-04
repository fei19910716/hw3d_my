#pragma once
#include "Graphics/Bindable/Bindable.h"
#include "Graphics/GraphicsThrowMacros.h"

template<typename C>
class ConstantBuffer: public Bindable{
public:
    ConstantBuffer(Graphics& gfx, UINT slot = 0u) noexcept: slot(slot){
        INFOMAN( gfx );

		//
		// first solution
		//
		// D3D11_BUFFER_DESC bd;
		// ZeroMemory(&bd, sizeof(bd));
		// bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		// bd.ByteWidth = sizeof(C);
		// bd.Usage = D3D11_USAGE_DEFAULT;
		// GetDevice( gfx )->CreateBuffer(&bd, 0, &pConstantBuffer);

		//
		// second solution
		//
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( C );
		cbd.StructureByteStride = 0u;
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,nullptr,pConstantBuffer.GetAddressOf() ) );

    }
    ConstantBuffer(Graphics& gfx, const C& consts, UINT slot = 0u) noexcept : slot(slot){
        INFOMAN( gfx );

		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof( consts );
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer( &cbd,&csd,&pConstantBuffer ) );
    }

    void Update(Graphics& gfx, const C& consts) {
        INFOMAN( gfx );

		//
		// first solution
		//
		// GetContext( gfx )->UpdateSubresource(pConstantBuffer.Get(), 0, 0, &consts, 0, 0);

		//
		// second solution
		//
		D3D11_MAPPED_SUBRESOURCE msr = {};
		GFX_THROW_INFO( GetContext( gfx )->Map(
			pConstantBuffer.Get(),0u,
			D3D11_MAP_WRITE_DISCARD,0u,
			&msr
		) );
		memcpy( msr.pData,&consts,sizeof( consts ) );
		GetContext( gfx )->Unmap( pConstantBuffer.Get(),0u );
    }

protected:
	UINT slot;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};



//
// Vertex constant buffer
//

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
	using ConstantBuffer<C>::slot;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override
	{
		GetContext( gfx )->VSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() );
	}
};


//
// Pixel constant buffer
//

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
	using ConstantBuffer<C>::slot;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind( Graphics& gfx ) noexcept override
	{
		GetContext( gfx )->PSSetConstantBuffers( slot,1u,pConstantBuffer.GetAddressOf() );
	}
};