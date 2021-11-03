#include "Texture.h"
#include "Graphics/Surface.h"
#include "Graphics/GraphicsThrowMacros.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const class Surface& s){
    INFOMAN(gfx);

    // create texture resource
    D3D11_TEXTURE2D_DESC td = {};
    td.Width = s.GetWidth();
    td.Height = s.GetHeight();
    td.MipLevels = 1;
    td.ArraySize = 1;
    td.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    td.SampleDesc.Count = 1;
    td.SampleDesc.Quality = 0;
    td.Usage = D3D11_USAGE_DEFAULT;
    td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    td.CPUAccessFlags = 0;
    td.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA sd= {};
    sd.pSysMem = s.GetBufferPtr();
    sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);

    wrl::ComPtr<ID3D11Texture2D> pTexture;
    GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
        &td,
        &sd,
        &pTexture
    ));


    // create resource view on the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = td.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = 1;
    GFX_THROW_INFO(GetDevice(gfx)->CreateShaderResourceView(
        pTexture.Get(),
        &srvDesc,
        &pTextureView
    ));

}

void Texture::Bind(Graphics& gfx) noexcept {

    GetContext(gfx)->PSSetShaderResources(
        0u,
        1u,
        pTextureView.GetAddressOf()
    );
}   