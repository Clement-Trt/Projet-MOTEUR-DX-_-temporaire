#include "pch.h"
#include "CameraSystem.h"
#include <DirectXMath.h>

DirectX::XMMATRIX CameraSystem::DefaultView(/*CameraComponent* cam*/)
{	
	DirectX::XMFLOAT3 position(0.0f, 0.0f, -5.0f);
		// Charger la position de la caméra dans un XMVECTOR
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);

	// Calcul du vecteur direction à partir des angles (m_Pitch et m_Yaw)
	float cosPitch = cosf(0.0f);
	float sinPitch = sinf(0.0f);
	float cosYaw = cosf(0.0f);
	float sinYaw = sinf(0.0f);

	// Le vecteur forward est calculé en utilisant des coordonnées sphériques.
	DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	return DirectX::XMMatrixLookToLH(pos, forward, up);
}

DirectX::XMMATRIX CameraSystem::GetViewMatrix(CameraComponent* camView, TransformComponent* transform)
{
	//if (transform == nullptr)
	//{
	//	// Charger la position de la caméra dans un XMVECTOR
	//	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_Position);

	//	// Calcul du vecteur direction à partir des angles (m_Pitch et m_Yaw)
	//	float cosPitch = cosf(m_Pitch);
	//	float sinPitch = sinf(m_Pitch);
	//	float cosYaw = cosf(m_Yaw);
	//	float sinYaw = sinf(m_Yaw);

	//	// Le vecteur forward est calculé en utilisant des coordonnées sphériques.
	//	DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

	//	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//	return DirectX::XMMatrixLookToLH(pos, forward, up);
	//}

	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transform->m_transform.matrix);
	return DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

void CameraSystem::SetViewMatrix(CameraComponent* camView, TransformComponent* transform)
{
	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transform->m_transform.matrix);
	camView->cameraView = DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

//void CameraSystem::SetViewMatrix(CameraComponent* camViewFrom, CameraComponent* camViewTo)
//{
//	camViewTo = camViewFrom;
//}
