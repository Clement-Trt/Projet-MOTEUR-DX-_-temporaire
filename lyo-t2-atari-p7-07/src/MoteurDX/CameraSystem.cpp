#include "pch.h"
#include "CameraSystem.h"
#include "InitDirect3DApp.h"

void CameraSystem::Initialize(InitDirect3DApp* gameManager)
{
	m_gameManager = gameManager;
}

void CameraSystem::Update()
{
	for (Entity* entity : m_gameManager->GetEntityManager()->GetEntityTab())
	{
		if (!entity)
			continue;
		if (!m_gameManager->GetEntityManager()->HasComponent(entity, COMPONENT_PLAYER)) // COMPONENT_PLAYER ou COMPONENT_CAMERA ?
			continue;

		TransformComponent* transformComponent = nullptr;
		CameraComponent* camComponent = nullptr;


		for (auto* component : m_gameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
		{
			if (component->ID == Transform_ID)
			{
				transformComponent = static_cast<TransformComponent*>(component);
			}
			if (component->ID == Camera_ID)
			{
				camComponent = static_cast<CameraComponent*>(component);
			}
		}
		if (transformComponent != nullptr && camComponent != nullptr)
		{
			camComponent->m_cameraTransform.vPosition = transformComponent->m_transform.vPosition;
			camComponent->m_cameraTransform.qRotation = transformComponent->m_transform.qRotation;
			camComponent->m_cameraTransform.UpdateMatrix();
			SetViewMatrix(m_gameManager->GetMainView(), &camComponent->m_cameraTransform);
		}
	}
}

DirectX::XMMATRIX CameraSystem::DefaultView()
{
	DirectX::XMFLOAT3 position(0.0f, 0.0f, 0.0f);
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

//DirectX::XMMATRIX CameraSystem::GetViewMatrix(CameraComponent* camView)
//{
//	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&camView->m_cameraTransform.GetMatrix());
//	return worldMatrix;
//}

void CameraSystem::SetViewMatrix(CameraComponent* camView, Transform* transform)
{
	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transform->GetMatrix());
	camView->m_cameraView = DirectX::XMMatrixInverse(nullptr, worldMatrix);

}
//void CameraSystem::SetViewMatrix(CameraComponent* camView, CameraComponent* playerCam)
//{
//
//	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&playerCam->m_cameraTransform.GetMatrix());
//	camView->m_cameraView = DirectX::XMMatrixInverse(nullptr, worldMatrix);
//}
