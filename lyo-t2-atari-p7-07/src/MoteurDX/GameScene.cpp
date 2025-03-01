#include "pch.h"

#include "GameScene.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraSystem.h"


void GameScene::CreateDefaultBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, int health)
{
	Entity* newIceBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newIceBlock);
	mpEntityManager->AddComponent<MeshComponent>(newIceBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newIceBlock);
	if (health != 0)
		mpEntityManager->AddComponent<HealthComponent>(newIceBlock);
	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"BoxTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(sizeX, sizeY, sizeZ);
			transform->m_transform.Move(posZ, posX, posY);
		}
		if (comp->ID == Health_ID)
		{
			HealthComponent* healthComp = static_cast<HealthComponent*>(comp);
			healthComp->maxHealth = healthComp->currentHealth = health;
		}
	}
}

void GameScene::CreateWallBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, int health)
{
	Entity* newIceBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newIceBlock);
	mpEntityManager->AddComponent<MeshComponent>(newIceBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newIceBlock);
	if (health != 0)
		mpEntityManager->AddComponent<HealthComponent>(newIceBlock);

	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"WallTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(sizeX, sizeY, sizeZ);
			transform->m_transform.Move(posZ, posX, posY);
		}
		if (comp->ID == Health_ID)
		{
			HealthComponent* healthComp = static_cast<HealthComponent*>(comp);
			healthComp->maxHealth = healthComp->currentHealth = health;
		}
	}
}




void GameScene::OnInitialize()
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
			transform->m_transform.Scale(0.5f, 0.5f, 0.5f);
			transform->m_transform.vPosition = { 0.0f , 0.0f, 0.0f };
		}
		if (component->ID == Camera_ID)
		{
			CameraComponent* cam = static_cast<CameraComponent*>(component);
			cam->m_cameraTransform.Scale(1.0f, 1.0f, 1.0f);
		}
	}
	playerEntity = entity1;


	Entity* skyBox = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(skyBox);
	mpEntityManager->AddComponent<MeshComponent>(skyBox);

	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[skyBox->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateSkyBoxCube();
			mesh->textureID = L"BoxTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(150, 150, 150);
			transform->m_transform.Move(0, 0, 0);
		}
	}
	
	CreateWallBlock(10, 15, 8, 20, 30, 40, 10);
	CreateWallBlock(12, 8, 5, -10, 50, 20, 20);
	CreateWallBlock(7, 14, 10, 5, -20, 35, 10);
	CreateWallBlock(15, 10, 6, 40, -30, 10, 20);
	CreateWallBlock(8, 12, 14, -25, 15, -40, 10);
	CreateWallBlock(10, 10, 10, 0, 0, 0, 20);
	CreateWallBlock(9, 20, 7, 30, -40, 25, 10);
	CreateWallBlock(14, 6, 12, -35, 10, 5, 20);
	CreateWallBlock(5, 8, 15, 50, 25, -20, 10);
	CreateWallBlock(12, 14, 9, -20, -10, 30, 20);
	CreateWallBlock(10, 5, 10, 15, 40, -35, 10);
	CreateWallBlock(7, 9, 12, -50, 20, 10, 20);
	CreateWallBlock(15, 10, 8, 25, -30, 45, 10);
	CreateWallBlock(6, 7, 14, -10, -50, 5, 20);
	CreateWallBlock(8, 12, 10, 35, 0, -25, 10);
	CreateWallBlock(10, 15, 6, -40, 30, 20, 20);
	CreateWallBlock(12, 8, 5, 10, -20, 50, 10);
	CreateWallBlock(9, 7, 14, -30, 45, -10, 20);
	CreateWallBlock(14, 6, 12, 5, -35, 10, 10);
	CreateWallBlock(5, 8, 15, 20, 50, -25, 20);
	CreateWallBlock(12, 14, 9, -10, -30, 40, 10);
	CreateWallBlock(10, 5, 10, 0, 15, -50, 20);
	CreateWallBlock(7, 9, 12, 25, -20, 30, 10);
	CreateWallBlock(15, 10, 8, -50, 35, 5, 20);
	CreateWallBlock(6, 7, 14, 40, 10, -45, 10);
	CreateWallBlock(8, 12, 10, -20, -10, 0, 20);
	CreateWallBlock(10, 15, 6, 50, 25, -35, 10);
	CreateWallBlock(12, 8, 5, -30, 40, 20, 20);
	CreateWallBlock(9, 7, 14, 5, -50, 10, 10);
	CreateWallBlock(14, 6, 12, -45, 15, 25, 20);
	CreateWallBlock(5, 8, 15, 30, -35, 40, 10);
	CreateWallBlock(12, 14, 9, -20, 0, -10, 20);
	CreateWallBlock(10, 5, 10, 15, -50, 30, 10);
	CreateWallBlock(7, 9, 12, -35, 20, -5, 20);
	CreateWallBlock(15, 10, 8, 40, -25, 50, 10);
	CreateWallBlock(6, 7, 14, -10, 30, -40, 20);
	CreateWallBlock(8, 12, 10, 50, -20, 0, 10);
	CreateWallBlock(10, 15, 6, -30, 5, -50, 20);
	CreateWallBlock(12, 8, 5, 25, -10, 35, 10);
	CreateWallBlock(9, 7, 14, -50, 15, -25, 20);
	CreateWallBlock(14, 6, 12, 10, -35, 5, 10);
	CreateWallBlock(5, 8, 15, -40, 20, 50, 20);
	CreateWallBlock(12, 14, 9, 30, -50, -10, 10);
	CreateWallBlock(10, 5, 10, -20, 40, 25, 20);
	CreateWallBlock(7, 9, 12, 50, -30, -35, 10);
	CreateWallBlock(15, 10, 8, -5, 25, 10, 20);
	CreateWallBlock(6, 7, 14, -50, 0, 20, 10);
	CreateWallBlock(8, 12, 10, 35, -40, 50, 20);
	CreateWallBlock(10, 15, 6, 5, 10, -30, 10);




	compteur = 50;
	compteur2 = 150;
}

void GameScene::OnUpdate()
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
				VelocityComponent* velComponent = nullptr;


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
					if (component->ID == Velocity_ID)
					{
						velComponent = static_cast<VelocityComponent*>(component);
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


				if (InputManager::GetKeyIsPressed('D')) velComponent->vx = 0.5f;
				if (InputManager::GetKeyIsPressed('Q')) velComponent->vx = -0.5f;

				if (InputManager::GetKeyIsPressed('Z')) velComponent->vz = 0.5f;
				if (InputManager::GetKeyIsPressed('S')) velComponent->vz = -0.5f;

				if (InputManager::GetKeyIsPressed('A')) velComponent->vy = 0.5f;
				if (InputManager::GetKeyIsPressed('E')) velComponent->vy = -0.5f;
				if (InputManager::GetKeyIsPressed('W'))
				{

					velComponent->vx *= 2;
					velComponent->vy *= 2;
					velComponent->vz *= 2;

				}

				mMvmt->Move(entity, velComponent, transform);
				mMvmt->SetVelocity(velComponent, 0, 0, 0);

				float posY = transform->m_transform.GetPositionY();
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
					camComponent->m_cameraTransform.vPosition = transform->m_transform.vPosition;
					camComponent->m_cameraTransform.qRotation = transform->m_transform.qRotation;
					//camComponent->m_cameraTransform.vScale = { 1.0f, 1.0f, 1.0f };
					camComponent->m_cameraTransform.UpdateMatrix();
					CameraSystem::SetViewMatrix(mpGameManager->GetMainView(), &camComponent->m_cameraTransform);
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
	if (InputManager::GetKeyIsPressed(MK_RBUTTON))
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
	TransformComponent* transform = nullptr;
	auto& playerComponents = mpGameManager->GetEntityManager()->GetComponentsTab()[playerEntity->tab_index]->vec_components;
	for (auto* component : playerComponents)
	{
		if (component->ID == Transform_ID)
		{
			transform = static_cast<TransformComponent*>(component);
			break;
		}
	}
	if (transform)
	{
		transform->m_transform.ResetRoll();
	}
}

void GameScene::OnClose()
{

}
