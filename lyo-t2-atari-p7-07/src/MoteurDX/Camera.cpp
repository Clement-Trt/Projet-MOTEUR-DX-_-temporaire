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

void Camera::Move(float rightDir, float upDir, float frontDir)
{
    m_Position.x += rightDir;
    m_Position.y += upDir;
    m_Position.z += frontDir;
}

void Camera::MoveRelative(float rightDir, float upDir, float frontDir)
{
    // Calculer le forward comme dans GetViewMatrix
    float cosPitch = cosf(m_Pitch);
    float sinPitch = sinf(m_Pitch);
    float cosYaw = cosf(m_Yaw);
    float sinYaw = sinf(m_Yaw);
    XMVECTOR forward = XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);
    // Calculer le right (vecteur perpendiculaire au forward et up)
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR right = XMVector3Cross(up, forward);

    // Convertir la position en XMVECTOR, ajouter les offsets, puis stocker dans m_Position.
    XMVECTOR pos = XMLoadFloat3(&m_Position);
    pos = XMVectorAdd(pos, XMVectorScale(right, rightDir));
    pos = XMVectorAdd(pos, XMVectorScale(up, upDir));
    pos = XMVectorAdd(pos, XMVectorScale(forward, frontDir));

    XMStoreFloat3(&m_Position, pos);
}


void Camera::Rotate(float pitch, float yaw)
{
    m_Pitch += pitch;
    m_Yaw += yaw;
}

XMMATRIX Camera::GetViewMatrix() const
{
    // Charger la position de la caméra dans un XMVECTOR
    XMVECTOR pos = XMLoadFloat3(&m_Position);
    // XMVECTOR pos = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 1.0f);

    // Calcul du vecteur direction à partir des angles (m_Pitch et m_Yaw)
    float cosPitch = cosf(m_Pitch);
    float sinPitch = sinf(m_Pitch);
    float cosYaw = cosf(m_Yaw);
    float sinYaw = sinf(m_Yaw);

    // Le vecteur forward est calculé en utilisant des coordonnées sphériques.
    XMVECTOR forward = XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);
    
    // Définir le target comme position + direction
    // XMVECTOR target = XMVectorAdd(pos, forward);

    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    
    return XMMatrixLookToLH(pos, forward, up);
    //return XMMatrixLookAtLH(pos, target, up);
}
