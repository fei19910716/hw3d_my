#pragma once
#include "Window.h"
#include "FTimer.h"

class App{
public:
    App();
    ~App();
    int Go();

private:
    void DoFrame();
private:
    Window wnd;
    FTimer timer;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};