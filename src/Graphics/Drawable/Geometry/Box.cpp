#include "Box.h"
#include "Graphics/Bindable/BindableBase.h"
#include "Graphics/GraphicsThrowMacros.h"
#include "Cube.h"

Box::Box( Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	std::uniform_real_distribution<float>& bdist,
	DirectX::XMFLOAT3 material  ) noexcept
	:
	TestObject( gfx,rng,adist,ddist,odist,rdist )
{
	namespace dx = DirectX;

	if(!IsStaticInitialized()) {
		struct Vertex
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};
		auto model = Cube::MakeIndependent<Vertex>();
		model.SetNormalIndependentFlat(  );

		AddStaticBind( std::make_unique<VertexBuffer>( gfx,model.vertices )	);

		auto pvs = std::make_unique<VertexShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PhongVS.hlsl" );
		auto pvsbc = pvs->GetByteCode();
		AddStaticBind( std::move( pvs ) );

		AddStaticBind( std::make_unique<PixelShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PhongPS.hlsl" ) );

		
		AddStaticBind( std::make_unique<IndexBuffer>( gfx,model.indices ) );

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
			{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		};
		AddStaticBind( std::make_unique<InputLayout>( gfx,ied,pvsbc ) );

		AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
	}
	AddBind( std::make_unique<TransformConstantBuffer>( gfx,*this ) );

	struct PSMaterialConstant{
		alignas(16) DirectX::XMFLOAT3 color;
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[2];
	} colorConstant;

	colorConstant.color = material;
	AddBind(std::make_unique<PixelConstantBuffer<PSMaterialConstant>>(gfx,colorConstant,1u));

		// model deformation transform (per instance, not stored as bind)
	dx::XMStoreFloat3x3(
		&mt,
		dx::XMMatrixScaling( 1.0f,1.0f,bdist( rng ) )
	);
}

DirectX::XMMATRIX Box::GetTransform() const noexcept
{
	namespace dx = DirectX;
	return dx::XMLoadFloat3x3( &mt ) * TestObject::GetTransform();
}