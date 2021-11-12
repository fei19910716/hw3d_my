#pragma once
#include "Window.h"
#include "FTimer.h"
#include "ImGuiManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Lighting/PointLight.h"
#include "Drawable/Model.h"
#include <set>
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

    float speed_factor = 1.0f;

    Model nano{ wnd.GetGraphics(),"D:\\GameEngine\\DirectX-Dev\\hw3d_my\\assets\\models\\suzanne.obj" };
};