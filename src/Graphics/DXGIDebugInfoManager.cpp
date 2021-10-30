#include "DXGIDebugInfoManager.h"
#include "Window.h"
#include "Graphics/Graphics.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")

#define GFX_THROW_NOINFO(hrcall) if( FAILED( hr = (hrcall) ) ) throw Graphics::HrException( __LINE__,__FILE__,hr )

DXGIDebugInfoManager::DXGIDebugInfoManager(){
    // define function signature of DXGIGetDebugInterface
    typedef HRESULT (WINAPI* DXGIGetDebugInterface)(REFIID, void**);

    const auto hModDxgiDebug = LoadLibraryEx(
        TEXT("dxgidebug.dll"),
        nullptr,
        LOAD_LIBRARY_SEARCH_SYSTEM32);
    if(hModDxgiDebug == nullptr){
        throw LAST_EXCEPTION();
    }
    // get address of DXGIGetDebugInterface in dll
    const auto DxgiGetDebugInterface = reinterpret_cast<DXGIGetDebugInterface>(
        reinterpret_cast<void*>(GetProcAddress(
            hModDxgiDebug,
            "DXGIGetDebugInterface"
        ))
    );
    if( DxgiGetDebugInterface == nullptr )
	{
		throw LAST_EXCEPTION();
	}
    // get the com ptr
    HRESULT hr;
    GFX_THROW_NOINFO( DxgiGetDebugInterface( 
        __uuidof(IDXGIInfoQueue),
        reinterpret_cast<void**>(&pDxgiInfoQueue) ) );

}

DXGIDebugInfoManager::~DXGIDebugInfoManager(){
    if( pDxgiInfoQueue != nullptr )
	{
		pDxgiInfoQueue->Release();
	}
}

void DXGIDebugInfoManager::Set() noexcept{
    next = pDxgiInfoQueue->GetNumStoredMessages(
        DXGI_DEBUG_ALL
    );
}

std::vector<std::string> DXGIDebugInfoManager::GetMessages() const{
    std::vector<std::string> messages;
    const auto end = pDxgiInfoQueue->GetNumStoredMessages(
        DXGI_DEBUG_ALL
    );
    for(auto i = next; i < end; i++){
        HRESULT hr;
        SIZE_T messageLength;
        GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(
            DXGI_DEBUG_ALL,
            i,
            nullptr,
            &messageLength
        ));
        auto bytes = std::make_unique<byte[]>(messageLength);
        auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(
            bytes.get()
        );
        GFX_THROW_NOINFO(pDxgiInfoQueue->GetMessage(
            DXGI_DEBUG_ALL,
            i,
            pMessage,
            &messageLength
        ));
        messages.push_back(pMessage->pDescription);
    }
    return messages;
}