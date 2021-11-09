#pragma once
#include "Graphics.h"

class Camera{
public:
    DirectX::XMMATRIX GetMatrix() const noexcept;
    void SpawnControlWindow() noexcept;
    void Reset() noexcept;

private:
    float r = 20.0f;
    float theta = 0.0f; // 控制绕物体的旋转
    float phi = 0.0f;
    float pitch = 0.0f; // 控制camera的旋转
    float yaw = 0.0f;
    float roll = 0.0f;
};