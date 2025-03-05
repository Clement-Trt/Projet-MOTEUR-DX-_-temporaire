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

    void SetViewMatrixT(DirectX::XMFLOAT4X4& transformMatrix) { m_matrix = transformMatrix; }
    void SetTransform(Transform* transformComponent) { mp_transform = transformComponent; }

private:
    DirectX::XMFLOAT3 m_position;
    float m_pitch, m_yaw; // Rotation de la camera

    DirectX::XMFLOAT4X4 m_matrix;

    Transform* mp_transform = nullptr;
};
