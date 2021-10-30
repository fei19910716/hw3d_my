#pragma once

#define EXCEPTION(hr) Window::HrException(__LINE__,__FILE__,hr)
#define LAST_EXCEPTION() Window::HrException(__LINE__,__FILE__,GetLastError())
#define NOGRAPHICS_EXCEPTION() Window::NoGraphicsException( __LINE__,__FILE__ )
