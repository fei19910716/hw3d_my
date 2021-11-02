#include "Bindable.h"

ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept{
    return gfx.pContext.Get();
}
ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept{
    return gfx.pDevice.Get();
}
DXGIDebugInfoManager& Bindable::GetInfoManager(Graphics& gfx) noexcept{
#ifndef NDEBUG
    return gfx.infoManager;
#else
    throw std::logic_error("try to access gfx.infoManager in Release config");
#endif
}