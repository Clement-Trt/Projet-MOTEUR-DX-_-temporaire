#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"
#include "Camera.h"
#include "InputManager.h"
#include "CameraSystem.h"


void SceneTest::CreateDefaultBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	Entity* newIceBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newIceBlock);
	mpEntityManager->AddComponent<MeshComponent>(newIceBlock);
	mpEntityManager->AddComponent<HealthComponent>(newIceBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newIceBlock);

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
			HealthComponent* health = static_cast<HealthComponent*>(comp);
			health->currentHealth = 20;
			health->maxHealth = 20;
		}
	}
}








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
	}
	playerEntity = entity1;

	//// 2
	//Entity* entity2 = mpEntityManager->CreateEntity();

	//mpEntityManager->AddComponent<TransformComponent>(entity2);
	//mpEntityManager->AddComponent<MeshComponent>(entity2);
	//mpEntityManager->AddComponent<ColliderComponent>(entity2);

	//for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity2->tab_index]->vec_components)
	//{
	//	if (component->ID == Mesh_ID)
	//	{
	//		MeshComponent* mesh = static_cast<MeshComponent*>(component);
	//		mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
	//		mesh->textureID = L"WallTexture"; // On assigne la texture
	//	}
	//	if (component->ID == Transform_ID)
	//	{
	//		TransformComponent* transform = static_cast<TransformComponent*>(component);
	//		transform->m_transform.Scale(3.0f, 1.0f, 1.0f);
	//		transform->m_transform.Move(3.0f, 3.0f, 3.0f);
	//	}
	//}


	Entity* skyBox = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(skyBox);
	mpEntityManager->AddComponent<MeshComponent>(skyBox);

	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[skyBox->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateSkyBoxCube();
			mesh->textureID = L"SkyBox";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(1000, 1000, 1000);
			transform->m_transform.Move(0, 0, 0);
		}
	}


	// Ennemy
	Entity* ennemy = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(ennemy);
	mpEntityManager->AddComponent<MeshComponent>(ennemy);
	mpEntityManager->AddComponent<ColliderComponent>(ennemy);
	mpEntityManager->AddComponent<EnnemyComponent>(ennemy);
	mpEntityManager->AddComponent<AttackComponent>(ennemy);
	mpEntityManager->AddComponent<HealthComponent>(ennemy);

	for (auto& comp : mpGameManager->GetEntityManager()->GetComponentToAddTab()[ennemy->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			mesh->textureID = L"DroneTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(2.f, 2.f, 2.f);
			transform->m_transform.Move(5, 5, 5);
		}
		if (comp->ID == Health_ID)
		{
			HealthComponent* healthComp = static_cast<HealthComponent*>(comp);
			healthComp->maxHealth = healthComp->currentHealth = 10;
		}
	}
	ennemyEntity = ennemy;

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

	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			float posX = 10 * i - 50;
			float posY = 10 * j;
			//float posZ = ;
			CreateDefaultBlock(8,8,8, posX, posY, 0);
		}
	}
	/*CreateDefaultBlock(1, 0.1, 0.1, 0, 0, 0);
	CreateDefaultBlock(0.1, 1, 0.1, 0, 0, 0);
	CreateDefaultBlock(0.1, 0.1, 1, 0, 0, 0);*/
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

				if (InputManager::GetKeyIsPressed('Q')) transform->m_transform.Move(0.0f, -0.4f, 0.0f);
				if (InputManager::GetKeyIsPressed('D')) transform->m_transform.Move(0.0f, 0.4, 0.0f);

				if (InputManager::GetKeyIsPressed('Z')) transform->m_transform.Move(0.4, 0.0f, 0.0f);
				if (InputManager::GetKeyIsPressed('S')) transform->m_transform.Move(-0.4f, 0.0f, 0.0f);

				if (InputManager::GetKeyIsPressed('A')) transform->m_transform.Move(0.0f, 0.0f, 0.4);
				if (InputManager::GetKeyIsPressed('E')) transform->m_transform.Move(0.0f, 0.0f, -0.4f);

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
					/*if (posY < 2)
					{
						transform->m_transform.GetPositionF3().y = 2;
					}
					if (posY > 5)
					{
						transform->m_transform.AddToGlobalPosY(-0.1f);
					}*/
					//CameraSystem::SetViewMatrix(mpGameManager->GetMainView(), transform);
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
			attack->targetEntity = ennemyEntity;
		}
	}
	if (InputManager::GetKeyIsPressed('W'))
	{
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
			transform->m_transform.ResetRotation();
		}
	}
}

void SceneTest::OnClose()
{

}
