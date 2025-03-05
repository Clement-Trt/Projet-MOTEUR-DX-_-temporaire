#include "pch.h"
#include "CameraSystem.h"
#include "InitDirect3DApp.h"

void CameraSystem::Initialize(InitDirect3DApp* gameManager)
{
	mp_gameManager = gameManager;
	m_viewMatrix = DefaultView();
}

void CameraSystem::Update()
{
	for (Entity* entity : mp_gameManager->GetEntityManager()->GetEntityTab())
	{
		if (!entity)
			continue;
		if (!mp_gameManager->GetEntityManager()->HasComponent(entity, COMPONENT_CAMERA))
			continue;

		TransformComponent* transformComponent = nullptr;
		CameraComponent* camComponent = nullptr;


		for (auto* component : mp_gameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
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

		if (FPS)
		{
			if (transformComponent != nullptr && camComponent != nullptr)
			{
				camComponent->m_cameraTransform.vPosition = transformComponent->m_transform.vPosition;
				camComponent->m_cameraTransform.qRotation = transformComponent->m_transform.qRotation;
				camComponent->m_cameraTransform.UpdateMatrix();
				SetViewMatrix(&camComponent->m_cameraTransform);
			}
		}

		if (TPS)
		{
			float distanceBehind = transformComponent->m_transform.vScale.z * 4.0f;
			float distanceUp = transformComponent->m_transform.vScale.z;

			float fdistanceBehind = 10 * 4.0f;
			float fdistanceUp = 10;

			DirectX::XMVECTOR localOffset;
			DirectX::XMVECTOR cubePos = DirectX::XMLoadFloat3(&transformComponent->m_transform.vPosition);
			DirectX::XMVECTOR eyePos;
			DirectX::XMVECTOR upDir;

			if (TPS_Lock == false)
			{
				DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transformComponent->m_transform.matrix);
				localOffset = DirectX::XMVectorSet(0.0f, distanceUp, -distanceBehind, 0.0f);
				eyePos = DirectX::XMVector3Transform(localOffset, worldMatrix);
				upDir = DirectX::XMLoadFloat3(&transformComponent->m_transform.vUp);
			}
			else
			{
				localOffset = DirectX::XMVectorSet(0.0f, fdistanceUp, -fdistanceBehind, 0.0f);
				eyePos = DirectX::XMVectorAdd(cubePos, localOffset);
				upDir = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			}

			DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePos, cubePos, upDir);

			SetViewMatrix(viewMatrix);
		}
	}
}

DirectX::XMMATRIX CameraSystem::DefaultView()
{
	DirectX::XMFLOAT3 position(0.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&position);

	float cosPitch = cosf(0.0f);
	float sinPitch = sinf(0.0f);
	float cosYaw = cosf(0.0f);
	float sinYaw = sinf(0.0f);

	DirectX::XMVECTOR forward = DirectX::XMVectorSet(cosPitch * sinYaw, sinPitch, cosPitch * cosYaw, 0.0f);

	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	return DirectX::XMMatrixLookToLH(pos, forward, up);
}

void CameraSystem::SetViewMatrix(Transform* transform)
{
	DirectX::XMMATRIX worldMatrix = DirectX::XMLoadFloat4x4(&transform->GetMatrix());
	m_viewMatrix = DirectX::XMMatrixInverse(nullptr, worldMatrix);
}

void CameraSystem::SetViewMatrix(DirectX::XMMATRIX viewMatrix)
{
	m_viewMatrix = viewMatrix;
}

