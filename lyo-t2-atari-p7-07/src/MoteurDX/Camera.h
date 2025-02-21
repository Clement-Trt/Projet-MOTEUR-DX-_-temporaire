#pragma once
#include <DirectXMath.h>
//#include "Transform.h"

class Transform;

class Camera
{
public:
    Camera();

    void SetPosition(float x, float y, float z);
    void Move(float rightDir, float upDir, float frontDir);
    void MoveRelative(float forwardDelta, float rightDelta, float upDelta);
    void Rotate(float pitch, float yaw);

    DirectX::XMMATRIX GetViewMatrix() const;

    DirectX::XMMATRIX GetViewMatrixT() const;

    void SetViewMatrixT(DirectX::XMFLOAT4X4& transformMatrix) { matrix = transformMatrix; }
    void SetTransform(Transform* transformComponent) { transform = transformComponent; }

private:
    DirectX::XMFLOAT3 m_Position;
    float m_Pitch, m_Yaw; // Rotation de la caméra

    DirectX::XMFLOAT4X4 matrix;

    Transform* transform = nullptr;
};
