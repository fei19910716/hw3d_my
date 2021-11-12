#pragma once
#include "Window.h"
#include "FTimer.h"
#include "ImGuiManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Lighting/PointLight.h"

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
    std::vector<std::unique_ptr<class Drawable>> drawables;
    std::vector<class Box*> boxes;
    static constexpr size_t nDrawables = 180;

    std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;
};