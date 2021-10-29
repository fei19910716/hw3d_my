#include "WindowsMessageMap.h"
#include <string>
#include <sstream>
#include <iomanip>


#define REGISTER_MESSAGE(msg) {msg,#msg}

WindowsMessageMap::WindowsMessageMap()
    :map_({
        REGISTER_MESSAGE(WM_CLOSE)
    })
{}

std::string WindowsMessageMap::operator()(DWORD msg, LPARAM lParam, WPARAM wParam) const{
    constexpr int firstColWidth = 25;
    const auto i = map_.find(msg);
    std::ostringstream oss;
    if(i !=map_.end()){
        oss << std::left << std::setw(firstColWidth) << i->second << std::right;
    }else{
        std::ostringstream padss;
        padss << "unknown message: 0x" << std::hex << msg;
        oss << std::left << std::setw(firstColWidth) << padss.str() << std::right;
    }
    oss << "    lParam: 0x" << std::hex << std::setfill('0') << std::setw(8) << lParam;
    oss << "    wParam: 0x" << std::hex << std::setfill('0') << std::setw(8) << wParam;
    oss << std::endl;

    return oss.str();
}