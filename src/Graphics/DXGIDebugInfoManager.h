#pragma once

#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <string>
#include <dxgidebug.h>
//
// 抓取驱动返回的错误信息
//
class DXGIDebugInfoManager{
public:
    DXGIDebugInfoManager();
    ~DXGIDebugInfoManager() = default;
    DXGIDebugInfoManager(const DXGIDebugInfoManager&) = delete;
    DXGIDebugInfoManager& operator=(const DXGIDebugInfoManager&) = delete;
    void Set() noexcept;
    std::vector<std::string> GetMessages() const;
private:
    unsigned long long next = 0u;
    Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue = nullptr;
};