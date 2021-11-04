#pragma once
#include "Window.h"
#include "FTimer.h"
#include "ImGuiManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Lighting/PointLight.h"
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
    PointLight light;
    std::vector<std::unique_ptr<class Drawable>> drawables;
    static constexpr size_t nDrawables = 180;
};