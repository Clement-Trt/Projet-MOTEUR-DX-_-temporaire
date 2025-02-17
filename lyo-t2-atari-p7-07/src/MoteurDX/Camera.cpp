#include "pch.h"
#include "Camera.h"
using namespace DirectX;

Camera::Camera()
    : m_Position(0.0f, 0.0f, -5.0f), m_Pitch(0.0f), m_Yaw(0.0f) {
}

void Camera::SetPosition(float x, float y, float z)
{
    m_Position = { x, y, z };
}

void Camera::Move(float dx, float dy, float dz)
{
    m_Position.x += dx;
    m_Position.y += dy;
    m_Position.z += dz;
}

void Camera::Rotate(float pitch, float yaw)
{
    m_Pitch += pitch;
    m_Yaw += yaw;
}

XMMATRIX Camera::GetViewMatrix() const
{
    // Calcul du vecteur direction à partir des angles (m_Pitch et m_Yaw)
    float cosPitch = cosf(m_Pitch);
    float sinPitch = sinf(m_Pitch);
    float cosYaw = cosf(m_Yaw);
    float sinYaw = sinf(m_Yaw);

    // Le vecteur forward
    XMVECTOR forward = XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

    XMVECTOR pos = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f);

    // XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    // Définir le target comme position + direction
    XMVECTOR target = XMVectorAdd(pos, forward);

    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    
    //return XMMatrixLookToLH(pos, target, up);
    return XMMatrixLookAtLH(pos, target, up);
}
