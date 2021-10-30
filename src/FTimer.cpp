#include "FTimer.h"

using namespace std::chrono;

FTimer::FTimer(){
    last = steady_clock::now();
}

float FTimer::Mark(){
    const auto old = last;
    last = steady_clock::now();
    const duration<float> frameTime = last - old;
    return frameTime.count();
}

float FTimer::Peek() const{
    return duration<float>(steady_clock::now() - last).count();
}