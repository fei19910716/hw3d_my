#include "FordException.h"
#include <sstream>

FordException::FordException(int line, const char* file) noexcept: line(line),file(file){

}

const char* FordException::what() const noexcept{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char* FordException::GetType() const noexcept{
    return "Ford Exception";
}

int FordException::GetLine() const noexcept{
    return line;
}

const std::string& FordException::GetFile() const noexcept{
    return file;
}


std::string FordException::GetOriginString() const noexcept{
    std::ostringstream oss;
    oss << "[File] " << file << std::endl
        << "[Line] " << line; 
    return oss.str();
}
