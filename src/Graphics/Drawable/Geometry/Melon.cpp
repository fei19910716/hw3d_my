#include "Melon.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GraphicsThrowMacros.h"
#include "Graphics/Drawable/Geometry/Sphere.h"


Melon::Melon( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_int_distribution<int>& longdist,
	std::uniform_int_distribution<int>& latdist )
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

	if( !IsStaticInitialized() )
	{
		auto pvs = std::make_unique<VertexShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\ColorIndexVS.hlsl" );
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\ColorIndexPS.hlsl" ) );

		struct PixelShaderConstants
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[8];
		};
		const PixelShaderConstants cb2 =
		{
			{
				{ 1.0f,1.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,0.0f,1.0f },
				{ 0.0f,1.0f,1.0f },
				{ 0.0f,0.0f,0.0f },
			}
		};
		AddStaticBind( std::make_unique<PixelConstantBuffer<PixelShaderConstants>>( gfx,cb2 ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}

	struct Vertex
	{
		dx::XMFLOAT3 pos;
	};
	auto model = Sphere::MakeTesselated<Vertex>( latdist( rng ),longdist( rng ) );
	// deform vertices of model by linear transformation
	model.Transform( dx::XMMatrixScaling( 1.0f,1.0f,1.2f ) );

	AddBind( std::make_unique<VertexBuffer>( gfx,model.vertices ) );

	AddBind( std::make_unique<IndexBuffer>( gfx,model.indices ) );

	AddBind( std::make_unique<TransformConstantBuffer>( gfx,*this ) );
}

void Melon::Update( float dt ) noexcept
{
	roll += droll * dt /10;
	pitch += dpitch * dt/10;
	yaw += dyaw * dt/10;
	theta += dtheta * dt/10;
	phi += dphi * dt/10;
	chi += dchi * dt/10;
}

DirectX::XMMATRIX Melon::GetTransform() const noexcept
{
	namespace dx = DirectX;
	return dx::XMMatrixRotationRollPitchYaw( pitch,yaw,roll ) *
		dx::XMMatrixTranslation( r,0.0f,0.0f ) *
		dx::XMMatrixRotationRollPitchYaw( theta,phi,chi ) *
		dx::XMMatrixTranslation( 0.0f,0.0f,20.0f );
}
