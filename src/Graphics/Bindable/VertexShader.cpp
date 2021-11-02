#include "VertexShader.h"
#include "Graphics/GraphicsThrowMacros.h"
#include <d3dcompiler.h>
VertexShader::VertexShader(Graphics& gfx, const LPCWSTR& path) noexcept
{
    INFOMAN(gfx);

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
        // 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
        // 但仍然允许着色器进行优化操作
        dwShaderFlags |= D3DCOMPILE_DEBUG;

        // 在Debug环境下禁用优化以避免出现一些不合理的情况
        dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
    ID3DBlob* errorBlob = nullptr;
    GFX_THROW_INFO( D3DCompileFromFile(
       path, 
       nullptr, 
       D3D_COMPILE_STANDARD_FILE_INCLUDE, 
       "main", 
       "vs_5_0",
        dwShaderFlags, 
        0,
        &pByteCodeBlob, 
        &errorBlob) );

	GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(
		pByteCodeBlob->GetBufferPointer(),
		pByteCodeBlob->GetBufferSize(),
		nullptr,
		&pVertexShader
	));
}

void VertexShader::Bind(Graphics& gfx) noexcept{
    GetContext(gfx)->VSSetShader(
		pVertexShader.Get(),
		nullptr,
		0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept{
    return pByteCodeBlob.Get();
}