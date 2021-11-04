#pragma once
#include "Window.h"
#include "FTimer.h"
#include "ImGuiManager.h"
#include "Graphics/Camera.h"
class App{
public:
    App();
    ~App();
    int Go();

private:
    void DoFrame();
private:
    ImGuiManager imgui;
    Window wnd;
    FTimer timer;
    Camera camera;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};