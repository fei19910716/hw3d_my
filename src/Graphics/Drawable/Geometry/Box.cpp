#include "Box.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GraphicsThrowMacros.h"
#include "Cone.h"

Box::Box( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist ) noexcept
	:
	r( rdist( rng ) ),
	droll( ddist( rng ) ),
	dpitch( ddist( rng ) ),
	dyaw( ddist( rng ) ),
	dphi( odist( rng ) ),
	dtheta( odist( rng ) ),
	dchi( odist( rng ) ),
	chi( adist( rng ) ),
	theta( adist( rng ) ),
	phi( adist( rng ) )
{
	namespace dx = DirectX;

	if(!IsStaticInitialized()) {
		struct Vertex
		{
			dx::XMFLOAT3 pos;
		};
		auto model = Cone::Make<Vertex>();
		model.Transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );

		AddStaticBind( std::make_unique<VertexBuffer>( gfx,model.vertices )	);

		auto pvs = std::make_unique<VertexShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\VertexShader.hlsl" );
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PixelShader.hlsl" ) );

		
		AddStaticBind( std::make_unique<IndexBuffer>( gfx,model.indices ) );

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};
		AddStaticBind( std::make_unique<PixelConstantBuffer<ConstantBuffer2>>( gfx,cb2 ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	AddBind( std::make_unique<TransformConstantBuffer>( gfx,*this ) );
}

void Box::Update( float dt ) noexcept
{
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	return  DirectX::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		    DirectX::XMMatrixTranslation( r,0.0f,0.0f ) *
		    DirectX::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
		    DirectX::XMMatrixTranslation( 0.0f,0.0f,20.0f );
}