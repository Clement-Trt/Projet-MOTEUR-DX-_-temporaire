#include "pch.h"
#include "Camera.h"
//using namespace DirectX;

Camera::Camera()
	: m_Position(0.0f, 0.0f, -5.0f), m_Pitch(0.0f), m_Yaw(0.0f) {
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position = { x, y, z };
}

void Camera::Move(float frontDir, float rightDir, float upDir)
{
	if (rightDir != 0.0f)
		m_Position.x = rightDir;

	if (upDir != 0.0f)
		m_Position.y = upDir;

	if (frontDir != 0.0f)
		m_Position.z = frontDir;
}

void Camera::MoveRelative(float frontDir, float rightDir, float upDir)
{
	// Calculer le forward comme dans GetViewMatrix
	float cosPitch = cosf(m_Pitch);
	float sinPitch = sinf(m_Pitch);
	float cosYaw = cosf(m_Yaw);
	float sinYaw = sinf(m_Yaw);
	// Calculer le right (vecteur perpendiculaire au forward et up)
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);

	// Convertir la position en XMVECTOR, ajouter les offsets, puis stocker dans m_Position.
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_Position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, rightDir));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(up, upDir));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, frontDir));

	DirectX::XMStoreFloat3(&m_Position, pos);
}


void Camera::Rotate(float pitch, float yaw)
{
	m_Pitch += pitch;
	m_Yaw += yaw;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	if (transform == nullptr)
	{
		// Charger la position de la caméra dans un XMVECTOR
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_Position);

		// Calcul du vecteur direction à partir des angles (m_Pitch et m_Yaw)
		float cosPitch = cosf(m_Pitch);
		float sinPitch = sinf(m_Pitch);
		float cosYaw = cosf(m_Yaw);
		float sinYaw = sinf(m_Yaw);

		// Le vecteur forward est calculé en utilisant des coordonnées sphériques.
		DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return DirectX::XMMatrixLookToLH(pos, forward, up);
	}

	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transform->matrix);
	return DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

DirectX::XMMATRIX Camera::GetViewMatrixT() const
{
	//XMMatrixLookToLH()
	return DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&matrix));
}
