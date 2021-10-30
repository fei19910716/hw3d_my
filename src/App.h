#pragma once
#include "Window.h"
#include "FTimer.h"

class App{
public:
    App();
    int Go();

private:
    void DoFrame();
private:
    Window wnd;
    FTimer timer;
};