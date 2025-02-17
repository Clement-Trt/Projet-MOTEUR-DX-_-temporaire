#pragma once
#include <DirectXMath.h>

class Camera
{
public:
    Camera();

    void SetPosition(float x, float y, float z);
    void Move(float dx, float dy, float dz);
    void Rotate(float pitch, float yaw);

    DirectX::XMMATRIX GetViewMatrix() const;

private:
    DirectX::XMFLOAT3 m_Position;
    float m_Pitch, m_Yaw; // Rotation de la caméra
};
