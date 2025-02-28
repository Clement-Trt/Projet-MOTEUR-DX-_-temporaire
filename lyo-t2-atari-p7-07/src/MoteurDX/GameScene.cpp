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
			mesh->textureID = L"IceTexture";
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
		//mpEntityManager->AddComponent<HealthComponent>(newIceBlock);

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
			transform->m_transform.Scale(20.0f, 20.0f, 20.0f);
			transform->m_transform.vPosition = { 1.0f , 10.0f, 1.0f };
		}
		if (component->ID == Camera_ID)
		{
			CameraComponent* cam = static_cast<CameraComponent*>(component);
			cam->m_cameraTransform.Scale(15.0f, 15.0f, 15.0f);
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
			transform->m_transform.Scale(5000, 5000, 5000);
			transform->m_transform.Move(0, 0, 0);
		}
	}

	CreateWallBlock(500, 300, 700, -2000, 1000, -1500, 20);
	CreateWallBlock(800, 600, 500, 500, -250, 2250, 20);
	CreateWallBlock(1000, 400, 1200, -1250, 1500, 1000, 20);
	CreateWallBlock(600, 900, 300, 1750, -1250, -2250, 20);
	CreateWallBlock(1200, 500, 600, -2500, 2000, 500, 20);
	CreateWallBlock(700, 700, 700, 1000, -1750, 1500, 20);
	CreateWallBlock(900, 400, 900, -500, 1250, -1750, 20);
	CreateWallBlock(400, 600, 1200, 2250, -1000, 1000, 20);
	CreateWallBlock(1100, 1100, 500, -1750, 500, -2250, 20);
	CreateWallBlock(1300, 500, 800, 250, -2000, 2500, 20);
	CreateWallBlock(500, 500, 500, 0, 0, 0, 20);
	CreateWallBlock(1400, 700, 700, -1250, 1500, -750, 20);
	CreateWallBlock(600, 600, 1100, 1250, -750, 1750, 20);
	CreateWallBlock(800, 1300, 400, -2250, 500, 2250, 20);
	CreateWallBlock(1000, 1000, 1000, 1500, -1500, -1500, 20);
	CreateWallBlock(700, 900, 700, -2500, 2000, -1000, 20);
	CreateWallBlock(500, 1200, 800, 500, -500, 500, 20);
	CreateWallBlock(1200, 800, 600, -1500, 250, 1500, 20);
	CreateWallBlock(400, 400, 400, 1000, -250, -1000, 20);
	CreateWallBlock(1300, 1300, 1300, -500, 1250, 2250, 20);
	CreateWallBlock(900, 1100, 900, 2250, -1000, -1250, 20);
	CreateWallBlock(1400, 600, 500, -1750, 500, 750, 20);
	CreateWallBlock(1100, 1400, 700, 250, -2000, -2500, 20);
	CreateWallBlock(800, 800, 800, -1250, 1500, 1750, 20);
	CreateWallBlock(1000, 600, 1200, 1250, -750, -1750, 20);
	CreateWallBlock(1200, 900, 900, -2250, 500, -500, 20);
	CreateWallBlock(500, 500, 1400, 1500, -1500, 250, 20);
	CreateWallBlock(700, 1200, 700, -2500, 2000, 1250, 20);
	CreateWallBlock(1400, 500, 1000, 500, -500, -250, 20);
	CreateWallBlock(800, 1100, 1100, -1500, 250, -2000, 20);
	CreateWallBlock(600, 1300, 1300, 1000, -250, 2250, 20);
	CreateWallBlock(900, 400, 1400, -500, 1250, -750, 20);
	CreateWallBlock(1100, 1400, 600, 2250, -1000, 1500, 20);
	CreateWallBlock(1000, 800, 500, -1750, 500, -1250, 20);
	CreateWallBlock(1200, 900, 700, 250, -2000, 2000, 20);
	CreateWallBlock(1300, 500, 800, -1250, 1500, -2250, 20);
	CreateWallBlock(700, 1300, 900, 1250, -750, 1000, 20);
	CreateWallBlock(1400, 1400, 1400, -2250, 500, 500, 20);
	CreateWallBlock(1100, 600, 1100, 1500, -1500, -1000, 20);
	CreateWallBlock(800, 1000, 1300, -2500, 2000, -1750, 20);
	CreateWallBlock(1000, 1000, 800, 500, -500, 750, 20);
	CreateWallBlock(900, 1400, 500, -1500, 250, 1750, 20);
	CreateWallBlock(1300, 700, 900, 1000, -250, -500, 20);
	CreateWallBlock(1200, 1100, 400, -500, 1250, 250, 20);
	CreateWallBlock(700, 1200, 1200, 2250, -1000, -2500, 20);
	CreateWallBlock(500, 500, 1100, -1750, 500, 1250, 20);
	CreateWallBlock(1000, 1300, 1300, 250, -2000, -750, 20);


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


				if (InputManager::GetKeyIsPressed('D')) velComponent->vx = 5.f;
				if (InputManager::GetKeyIsPressed('Q')) velComponent->vx = -5.f;

				if (InputManager::GetKeyIsPressed('Z')) velComponent->vz = 5.f;
				if (InputManager::GetKeyIsPressed('S')) velComponent->vz = -5.f;

				if (InputManager::GetKeyIsPressed('A')) velComponent->vy = 5.f;
				if (InputManager::GetKeyIsPressed('E')) velComponent->vy = -5.f;
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
