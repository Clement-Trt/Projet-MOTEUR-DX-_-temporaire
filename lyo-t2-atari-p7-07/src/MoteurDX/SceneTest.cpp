#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraSystem.h"

void SceneTest::OnInitialize()
{
	// 1
	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(entity1);
	mpEntityManager->AddComponent<MeshComponent>(entity1);
	mpEntityManager->AddComponent<VelocityComponent>(entity1);
	mpEntityManager->AddComponent<CameraComponent>(entity1);
	mpEntityManager->AddComponent<AttackComponent>(entity1);
	mpEntityManager->AddComponent<ColliderComponent>(entity1);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"PlayerTexture"; // On assigne la texture
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(1.0f, 1.0f, 1.0f);
		}/*
		if (component->ID == Camera_ID)
		{
			CameraComponent* cam = static_cast<CameraComponent*>(component);
			mpGameManager->SetCamera(&cam->m_camera);
		}*/
		if (component->ID == Collider_ID)
		{
			ColliderComponent* collider = static_cast<ColliderComponent*>(component);
			collider->m_isSolide = true;
			collider->m_isDynamic = true;
		}
	}
	playerEntity = entity1;

	// 2
	Entity* entity2 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(entity2);
	mpEntityManager->AddComponent<MeshComponent>(entity2);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity2->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"WallTexture"; // On assigne la texture
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(3.0f, 1.0f, 1.0f);
			transform->m_transform.Move(3.0f, 3.0f, 3.0f);
		}
	}

	// 3
	Entity* entityBox = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(entityBox);
	mpEntityManager->AddComponent<MeshComponent>(entityBox);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entityBox->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"BoxTexture"; // On assigne la texture
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(1.0f, 1.0f, 1.0f);
			transform->m_transform.Move(-3.0f, -3.0f, -3.0f);
		}
	}

	Entity* entityIceBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(entityIceBlock);
	mpEntityManager->AddComponent<MeshComponent>(entityIceBlock);
	mpEntityManager->AddComponent<HealthComponent>(entityIceBlock);
	mpEntityManager->AddComponent<ColliderComponent>(entityIceBlock);

	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entityIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"IceTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(1.0f, 1.0f, 1.0f);
			transform->m_transform.Move(5.0f, 0.0f, 0.0f);
		}
		if (comp->ID == Health_ID)
		{
			HealthComponent* health = static_cast<HealthComponent*>(comp);
			health->currentHealth = 100;
			health->maxHealth = 100;
		}
		if (comp->ID == Collider_ID)
		{
			ColliderComponent* collider = static_cast<ColliderComponent*>(comp);
			collider->m_isSolide = true;
		}
	}
	iceBlockEntity = entityIceBlock;


	// 2
	Entity* floor = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(floor);
	mpEntityManager->AddComponent<MeshComponent>(floor);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[floor->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"WallTexture"; // On assigne la texture
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(200.f, 1.0f, 200.f);
			transform->m_transform.Move(0.0f, 0.0f, -1.0f);
		}
	}


	compteur = 50;
	compteur2 = 150;
}

void SceneTest::OnUpdate()
{
	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
		{
			continue;
		}
		if (mpEntityManager->HasComponent(entity, COMPONENT_CAMERA))
		{
			// Update de position player
			if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_CAMERA))
			{
				TransformComponent* transform = nullptr;
				CameraComponent* camComponent = nullptr;
				ColliderComponent* collider = nullptr;

				for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
				{
					if (component->ID == Transform_ID)
					{
						transform = static_cast<TransformComponent*>(component);
					}
					if (component->ID == Camera_ID)
					{
						camComponent = static_cast<CameraComponent*>(component);
					}
					if (component->ID == Collider_ID)
					{
						collider = static_cast<ColliderComponent*>(component);
					}
				}


				// Mettez a jour la souris en passant le handle de la fenetre
				InputManager::UpdateMouse(GetActiveWindow());

				// Recuperer le deplacement de la souris
				int deltaX = InputManager::GetMouseDeltaX();
				int deltaY = InputManager::GetMouseDeltaY();

				// Sensibilite de la souris
				const float sensitivity = 0.005f;
				if (InputManager::GetKeyIsPressed(MK_LBUTTON))
				{
					// Mettre a jour la rotation de la camera en fonction du delta
					transform->m_transform.Rotation(0.0f, deltaY * sensitivity, deltaX * sensitivity);
				}

				if (InputManager::GetKeyIsPressed('Q')) transform->m_transform.Move(0.0f, -0.1f, 0.0f);
				if (InputManager::GetKeyIsPressed('D')) transform->m_transform.Move(0.0f, 0.1f, 0.0f);

				if (InputManager::GetKeyIsPressed('Z')) transform->m_transform.Move(0.1f, 0.0f, 0.0f);
				if (InputManager::GetKeyIsPressed('S')) transform->m_transform.Move(-0.1f, 0.0f, 0.0f);

				if (InputManager::GetKeyIsPressed('A')) transform->m_transform.Move(0.0f, 0.0f, 0.1f);
				if (InputManager::GetKeyIsPressed('E')) transform->m_transform.Move(0.0f, 0.0f, -0.1f);

				float posY = transform->m_transform.vPosition.y;
				/*wchar_t title[256];
				swprintf_s(title, 256, L"lFrontDir = %f", posY);
				SetWindowText(GetActiveWindow(), title);*/


				/*DWORD t = timeGetTime();
				DWORD dt = timeGetTime() - t;*/
				/*wchar_t title[256];
				swprintf_s(title, 256, L"lFrontDir = ", transform->m_transform.GetPositionZ());
				SetWindowText(GetActiveWindow(), title);*/

				if (transform != nullptr && camComponent != nullptr)
				{
					if (posY < 0.5)
					{
						transform->m_transform.vPosition.y = 0.5;
					}
					if (posY > 2)
					{
						transform->m_transform.vPosition.y -= 0.01;
					}
					CameraSystem::SetViewMatrix(mpGameManager->GetMainView(), transform);
				}
			}
		}
		else
		{
			if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY))
			{
				TransformComponent* transform = nullptr;
				VelocityComponent* vel = nullptr;


				for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
				{
					if (component->ID == Transform_ID)
					{
						transform = static_cast<TransformComponent*>(component);
					}
					if (component->ID == Velocity_ID)
					{
						vel = static_cast<VelocityComponent*>(component);
					}
				}
				if (transform != nullptr && vel != nullptr)
				{
					mMvmt->Move(entity, vel, transform);
				}
				//mMvmt->SetVelocity(vel, transform->m_transform.)
			}
		}
	}

	// Si la touche 'P' est presse, on demande une attaque du joueur sur l'IceBlock
	if (InputManager::GetKeyIsPressed('P'))
	{
		AttackComponent* attack = nullptr;
		auto& playerComponents = mpGameManager->GetEntityManager()->GetComponentsTab()[playerEntity->tab_index]->vec_components;
		for (auto* component : playerComponents)
		{
			if (component->ID == Attack_ID)
			{
				attack = static_cast<AttackComponent*>(component);
				break;
			}
		}
		if (attack)
		{
			// Declencher l'attaque en definissant le flag et en indiquant la cible
			attack->attackRequested = true;
			attack->targetEntity = iceBlockEntity;
		}
	}

}

void SceneTest::OnClose()
{

}
