#include "pch.h"
#include "Camera.h"
//using namespace DirectX;

Camera::Camera()
	: m_position(0.0f, 0.0f, -5.0f), m_pitch(0.0f), m_yaw(0.0f) {
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = { x, y, z };
}

void Camera::Move(float frontDir, float rightDir, float upDir)
{
	if (rightDir != 0.0f)
		m_position.x = rightDir;

	if (upDir != 0.0f)
		m_position.y = upDir;

	if (frontDir != 0.0f)
		m_position.z = frontDir;
}

void Camera::MoveRelative(float frontDir, float rightDir, float upDir)
{
	// Calculer le forward comme dans GetViewMatrix
	float cosPitch = cosf(m_pitch);
	float sinPitch = sinf(m_pitch);
	float cosYaw = cosf(m_yaw);
	float sinYaw = sinf(m_yaw);
	// Calculer le right (vecteur perpendiculaire au forward et up)
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR right = DirectX::XMVector3Cross(up, forward);

	// Convertir la position en XMVECTOR, ajouter les offsets, puis stocker dans m_position.
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(right, rightDir));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(up, upDir));
	pos = DirectX::XMVectorAdd(pos, DirectX::XMVectorScale(forward, frontDir));

	DirectX::XMStoreFloat3(&m_position, pos);
}


void Camera::Rotate(float pitch, float yaw)
{
	m_pitch += pitch;
	m_yaw += yaw;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	if (mp_transform == nullptr)
	{
		// Charger la position de la camera dans un XMVECTOR
		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);

		// Calcul du vecteur direction a partir des angles (m_pitch et m_yaw)
		float cosPitch = cosf(m_pitch);
		float sinPitch = sinf(m_pitch);
		float cosYaw = cosf(m_yaw);
		float sinYaw = sinf(m_yaw);

		// Le vecteur forward est calcul en utilisant des coordonnees spheriques.
		DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

		DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		return DirectX::XMMatrixLookToLH(pos, forward, up);
	}

	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&mp_transform->GetMatrix());
	return DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

DirectX::XMMATRIX Camera::GetViewMatrixT() const
{
	//XMMatrixLookToLH()
	return DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&m_matrix));
}
