#pragma once
#include <chrono>

class FTimer{
public:
    FTimer();
    float Mark();
    float Peek() const;
private:
    std::chrono::steady_clock::time_point last;
};