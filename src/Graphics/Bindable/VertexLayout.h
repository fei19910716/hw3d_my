#pragma once
#include <vector>
#include <DirectXMath.h>
#include <type_traits>

struct BGRAColor
{
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class VertexLayout
{
public:
    // 顶点属性type
	enum ElementType
	{
		Position2D,
		Position3D,
		Texture2D,
		Normal,
		Float3Color,
		Float4Color,
		BGRAColor,
	};
    // 封装每个属性的type 和offset
	class Element
	{
	public:
		Element( ElementType type,size_t offset )
			:
			type( type ),
			offset( offset )
		{}
		size_t GetOffsetAfter() const noexcept
		{
			return offset + Size();
		}
		size_t GetOffset() const
		{
			return offset;
		}
		size_t Size() const noexcept
		{
			return SizeOf( type );
		}
		static constexpr size_t SizeOf( ElementType type ) noexcept
		{
			using namespace DirectX;
			switch( type )
			{
			case Position2D:
				return sizeof( XMFLOAT2 );
			case Position3D:
				return sizeof( XMFLOAT3 );
			case Texture2D:
				return sizeof( XMFLOAT2 );
			case Normal:
				return sizeof( XMFLOAT3 );
			case Float3Color:
				return sizeof( XMFLOAT3 );
			case Float4Color:
				return sizeof( XMFLOAT3 );
			case BGRAColor:
				return sizeof( unsigned int );
			}
			assert( "Invalid element type" && false );
			return 0u;
		}
		ElementType GetType() const noexcept
		{
			return type;
		}
	private:
		ElementType type;
		size_t offset;
	};
public:
    // 获取某个type的element
	template<ElementType Type>
	const Element& Resolve() const noexcept
	{
		for( auto& e : elements )
		{
			if( e.GetType() == Type )
			{
				return e;
			}
		}
		assert( "Could not resolve element type" && false );
		return elements.front();
	}
	const Element& ResolveByIndex( size_t i ) const noexcept
	{
		return elements[i];
	}
	template<ElementType Type>
	VertexLayout& Append() noexcept
	{
		elements.emplace_back( Type,Size() ); // 通过type和offset进行构造
		return *this;
	}
	size_t Size() const noexcept
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}
private:
	std::vector<Element> elements; // vertexLayout 维护所有的属性
};

// 封装顶点信息
class Vertex
{
	friend class VertexBuffer;
public:
    // 获取某个属性
	template<VertexLayout::ElementType Type>
	auto& Attr() noexcept
	{
		using namespace DirectX;
		const auto& element = layout.Resolve<Type>();
		auto pAttribute = pData + element.GetOffset();
		if constexpr( Type == VertexLayout::Position2D )
		{
			return *reinterpret_cast<XMFLOAT2*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::Position3D )
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::Texture2D )
		{
			return *reinterpret_cast<XMFLOAT2*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::Normal )
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::Float3Color )
		{
			return *reinterpret_cast<XMFLOAT3*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::Float4Color )
		{
			return *reinterpret_cast<XMFLOAT4*>(pAttribute);
		}
		else if constexpr( Type == VertexLayout::BGRAColor )
		{
			return *reinterpret_cast<BGRAColor*>(pAttribute);
		}
		else
		{
			assert( "Bad element type" && false );
			return *reinterpret_cast<char*>(pAttribute);
		}
	}
    // 设置某个属性的value
	template<typename T>
	void SetAttributeByIndex( size_t i,T&& val ) noexcept
	{
		using namespace DirectX;
		const auto& element = layout.ResolveByIndex( i );
		auto pAttribute = pData + element.GetOffset();
		switch( element.GetType() )
		{
		case VertexLayout::Position2D:
			SetAttribute<XMFLOAT2>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::Position3D:
			SetAttribute<XMFLOAT3>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::Texture2D:
			SetAttribute<XMFLOAT2>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::Normal:
			SetAttribute<XMFLOAT3>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::Float3Color:
			SetAttribute<XMFLOAT3>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::Float4Color:
			SetAttribute<XMFLOAT4>( pAttribute,std::forward<T>( val ) );
			break;
		case VertexLayout::BGRAColor:
			SetAttribute<BGRAColor>( pAttribute,std::forward<T>( val ) );
			break;
		default:
			assert( "Bad element type" && false );
		}	
	}
private:
    // Vertex由字节流和描述信息构造
	Vertex( char* pData,const VertexLayout& layout ) noexcept
		:
		pData( pData ),
		layout( layout )
	{
		assert( pData != nullptr );
	}
	template<typename First,typename ...Rest>
	// enables parameter pack setting of multiple parameters by element index
	void SetAttributeByIndex( size_t i,First&& first,Rest&&... rest ) noexcept
	{
		SetAttributeByIndex( i,std::forward<First>( first ) );
		SetAttributeByIndex( i,std::forward<Rest>( rest )... );
	}
	// helper to reduce code duplication in SetAttributeByIndex
	template<typename Dest,typename Src>
	void SetAttribute( char* pAttribute,Src&& val ) noexcept
	{
		if constexpr( std::is_assignable<Dest,Src>::value )
		{
			*reinterpret_cast<Dest*>(pAttribute) = val;
		}
		else
		{
			assert( "Parameter attribute type mismatch" && false );
		}
	}
private:
	char* pData = nullptr;
	const VertexLayout& layout;
};

class VertexBuffer
{
public:
	VertexBuffer( VertexLayout layout ) noexcept
		:
		layout( std::move( layout ) )
	{}
	const VertexLayout& GetLayout() const noexcept
	{
		return layout;
	}
	size_t Size() const noexcept
	{
		return buffer.size() / layout.Size();
	}
	template<typename ...Params>
	void EmplaceBack( Params&&... params ) noexcept
	{
		buffer.resize( buffer.size() + layout.Size() );
		Back().SetAttributeByIndex( 0u,std::forward<Params>( params )... );
	}
	Vertex Back() noexcept
	{
		assert( buffer.size() != 0u );
		return Vertex{ buffer.data() + buffer.size() - layout.Size(),layout };
	}
	Vertex Front() noexcept
	{
		assert( buffer.size() != 0u );
		return Vertex{ buffer.data(),layout };
	}
	Vertex operator[]( size_t i ) noexcept
	{
		assert( i < Size() );
		return Vertex{ buffer.data() + layout.Size() * i,layout };
	}
private:
	std::vector<char> buffer;
	VertexLayout layout;
};