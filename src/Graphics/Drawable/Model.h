#pragma once
#include "DrawableBase.h"
#include "Bindable/BindableBase.h"
#include "Bindable/VertexLayout.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//
// 对obj模型的封装
class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh( Graphics& gfx,std::vector<std::unique_ptr<Bindable>> bindPtrs )
	{
		if( !IsStaticInitialized() )
		{
			AddStaticBind( std::make_unique<Topology>( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
		}

		for( auto& pb : bindPtrs )
		{
			if( auto pi = dynamic_cast<IndexBuffer*>(pb.get()) )
			{
				AddStaticBind( std::move( pb ) );
			}
			else
			{
				AddBind( std::move( pb ) );
			}
		}

		AddBind( std::make_unique<TransformConstantBuffer>( gfx,*this ) );
	}
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noexcept
	{
		DirectX::XMStoreFloat4x4( &transform,accumulatedTransform );
		Drawable::Draw( gfx );
	}
	DirectX::XMMATRIX GetTransform() const noexcept override
	{
		return DirectX::XMLoadFloat4x4( &transform );
	}
private:
	mutable DirectX::XMFLOAT4X4 transform;
};

class Node
{
	friend class Model;
public:
	Node( std::vector<Mesh*> meshPtrs,const DirectX::XMMATRIX& transform ) noexcept
		:
		meshPtrs( std::move( meshPtrs ) )
	{
		DirectX::XMStoreFloat4x4( &this->transform,transform );
	}
	void Draw( Graphics& gfx,DirectX::FXMMATRIX accumulatedTransform ) const noexcept
	{
		const auto built = DirectX::XMLoadFloat4x4( &transform ) * accumulatedTransform;
		for( const auto pm : meshPtrs )
		{
			pm->Draw( gfx,built );
		}
		for( const auto& pc : childPtrs )
		{
			pc->Draw( gfx,built );
		}
	}
private:
	void AddChild( std::unique_ptr<Node> pChild ) noexcept
	{
		assert( pChild );
		childPtrs.push_back( std::move( pChild ) );
	}
private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	DirectX::XMFLOAT4X4 transform;
};

class Model
{
public:
	Model( Graphics& gfx,const std::string fileName )
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile( fileName.c_str(),
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices
		);

		for( size_t i = 0; i < pScene->mNumMeshes; i++ )
		{
			meshPtrs.push_back( ParseMesh( gfx,*pScene->mMeshes[i] ) );
		}

		pRoot = ParseNode( *pScene->mRootNode );
	}
	static std::unique_ptr<Mesh> ParseMesh( Graphics& gfx,const aiMesh& mesh )
	{
		namespace dx = DirectX;
		using fordexp::VertexLayout;

		fordexp::VertexBuffer vbuf( std::move(
			VertexLayout{}
			.Append( VertexLayout::Position3D )
			.Append( VertexLayout::Normal )
		) );

		for( unsigned int i = 0; i < mesh.mNumVertices; i++ )
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mVertices[i]),
				*reinterpret_cast<dx::XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve( mesh.mNumFaces * 3 );
		for( unsigned int i = 0; i < mesh.mNumFaces; i++ )
		{
			const auto& face = mesh.mFaces[i];
			assert( face.mNumIndices == 3 );
			indices.push_back( face.mIndices[0] );
			indices.push_back( face.mIndices[1] );
			indices.push_back( face.mIndices[2] );
		}

		std::vector<std::unique_ptr<Bindable>> bindablePtrs;

		bindablePtrs.push_back( std::make_unique<VertexBuffer>( gfx,vbuf ) );

		bindablePtrs.push_back( std::make_unique<IndexBuffer>( gfx,indices ) );

		auto pvs = std::make_unique<VertexShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PhongVS.hlsl" );
		auto pvsbc = pvs->GetByteCode();
		bindablePtrs.push_back( std::move( pvs ) );

		bindablePtrs.push_back( std::make_unique<PixelShader>( gfx,L"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\shaders\\PhongPS.hlsl" ) );

		bindablePtrs.push_back( std::make_unique<InputLayout>( gfx,vbuf.GetLayout().GetD3DLayout(),pvsbc ) );

		struct PSMaterialConstant
		{
			DirectX::XMFLOAT3 color = { 0.6f,0.6f,0.8f };
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<PSMaterialConstant>>( gfx,pmc,1u ) );

		return std::make_unique<Mesh>( gfx,std::move( bindablePtrs ) );
	}
	std::unique_ptr<Node> ParseNode( const aiNode& node )
	{
		namespace dx = DirectX;
		const auto transform = dx::XMMatrixTranspose( dx::XMLoadFloat4x4(
			reinterpret_cast<const dx::XMFLOAT4X4*>(&node.mTransformation)
		) );

		std::vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve( node.mNumMeshes );
		for( size_t i = 0; i < node.mNumMeshes; i++ )
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back( meshPtrs.at( meshIdx ).get() );
		}

		auto pNode = std::make_unique<Node>( std::move( curMeshPtrs ),transform );
		for( size_t i = 0; i < node.mNumChildren; i++ )
		{
			pNode->AddChild( ParseNode( *node.mChildren[i] ) );
		}
		
		return pNode;
	}
	void Draw( Graphics& gfx ) const
	{
		pRoot->Draw( gfx,DirectX::XMMatrixIdentity() );
	}
private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::unique_ptr<Mesh>> meshPtrs;
};