#include "Triangle.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GraphicsThrowMacros.h"

Triangle::Triangle( Graphics& gfx) noexcept
	: angle(10.0)
{
	struct Vertex
	{
		float x;
		float y;
		float z;
	};

	// create vertex buffer (1 2d triangle at center of screen)
	const std::vector<Vertex> vertices =
	{
		{ 0.5f,0.5f,0.0f },
		{ 0.5f,-0.5f,0.0f },
		{ -0.5f,-0.5f,0.0f },
	};

	AddBind( std::make_unique<VertexBuffer>( gfx,vertices )	);

	auto pvs = std::make_unique<VertexShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\VertexShader.hlsl" );
	auto pvsbc = pvs->GetByteCode();
	AddBind( std::move( pvs ) );

	AddBind( std::make_unique<PixelShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PixelShader.hlsl" ) );

	const std::vector<unsigned short> indices =
	{
		0,1,2
	};
	AddBind( std::make_unique<IndexBuffer>( gfx,indices ) );

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

	AddBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	AddBind( std::make_unique<TransformConstantBuffer>( gfx,*this) );
}

void Triangle::Update( float dt ) noexcept
{
    angle = dt;
}

DirectX::XMMATRIX Triangle::GetTransform() const noexcept
{
	return  DirectX::XMMatrixTranslation( 0.0f,0.0f,10.0f );
}