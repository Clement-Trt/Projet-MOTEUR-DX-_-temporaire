#include "pch.h"

#include "GameScene.h"
#include "EntityManager.h"
#include "InitDirect3DApp.h"
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
	for (auto& comp : mpEntityManager->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
			mesh->m_textureID = L"BoxTexture";
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

		for (auto& comp : mpEntityManager->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
		{
			if (comp->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(comp);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
				mesh->m_textureID = L"WallTexture";
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
	// entityPlayer
	{
		Entity* entityPlayer = mpEntityManager->CreateEntity();

		mpEntityManager->AddComponent<PlayerComponent>(entityPlayer);
		mpEntityManager->AddComponent<TransformComponent>(entityPlayer);
		mpEntityManager->AddComponent<MeshComponent>(entityPlayer);
		mpEntityManager->AddComponent<VelocityComponent>(entityPlayer);
		mpEntityManager->AddComponent<CameraComponent>(entityPlayer);
		mpEntityManager->AddComponent<AttackComponent>(entityPlayer);
		mpEntityManager->AddComponent<ColliderComponent>(entityPlayer);
		mpEntityManager->AddComponent<LightComponent>(entityPlayer);


		for (auto& component : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[entityPlayer->tab_index]->vec_components)
		{
			if (component->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(component);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
				mesh->m_textureID = L"PlayerTexture"; // On assigne la texture
			}
			if (component->ID == Light_ID)
			{
				LightComponent* light = static_cast<LightComponent*>(component);
				light->type = LightType::Point;
				light->Position = { 0.0f, 0.0f, 0.0f };
				light->Color = { 15.0f, 15.0f, 15.0f };
				light->ConstantAtt = 1.0f;
				light->LinearAtt = 0.09f;
				light->QuadraticAtt = 0.032f;
			}
			if (component->ID == Transform_ID)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(component);
				transform->m_transform.Scale(3.0f, 3.0f, 3.0f);
				transform->m_transform.vPosition = { 0.0f , 0.0f, 0.0f };
			}
			if (component->ID == Camera_ID)
			{
				CameraComponent* cam = static_cast<CameraComponent*>(component);
				cam->m_cameraTransform.Scale(1.0f, 1.0f, 1.0f);
			}
			if (component->ID == Attack_ID) 
			{
				AttackComponent* attack = static_cast<AttackComponent*>(component);
				attack->projectileTexture = L"BlueBeamTexture";
				attack->attackCooldown = 0.1f;
				attack->damage = 2;

				attack->projectileSpeed = 1;
				attack->projectileSizeX = 0.2f;
				attack->projectileSizeY = 0.2f;
				attack->projectileSizeZ = 1.0f;
			}
		}
		mp_playerEntity = entityPlayer;
	}

	// entitySkyBox
	{
		Entity* entitySkyBox = mpEntityManager->CreateEntity();
		mpEntityManager->AddComponent<TransformComponent>(entitySkyBox);
		mpEntityManager->AddComponent<MeshComponent>(entitySkyBox);

		for (auto& comp : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[entitySkyBox->tab_index]->vec_components)
		{
			if (comp->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(comp);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateSkyBoxCube();
				mesh->m_textureID = L"SkyBox2";
			}
			if (comp->ID == Transform_ID)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(comp);
				transform->m_transform.Scale(300, 300, 300);
				transform->m_transform.Move(0, 0, 0);
			}
		}
	}

	// Blocks
	{
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
	}

	// Lumiere directionnel : 
	//{
	//	Entity* sunEntity = mpEntityManager->CreateEntity();
	//	mpEntityManager->AddComponent<LightComponent>(sunEntity);

	//	for (auto& component : m_gameManager->GetEntityManager()->GetComponentToAddTab()[sunEntity->tab_index]->vec_components)
	//	{
	//		if (component->ID == Light_ID)
	//		{
	//			LightComponent* light = static_cast<LightComponent*>(component);
	//			light->type = LightType::Directional;
	//			light->Direction = { 0.57735f, -0.57735f, 0.57735f };
	//			light->Color = { 1.0f, 1.0f, 0.95f };
	//		}
	//	}

	//}
		
	mp_camera->SetFPS();
}

void GameScene::OnUpdate()
{
	TransformComponent* transform = nullptr;
	VelocityComponent* velComponent = nullptr;


	for (auto* component : mpEntityManager->GetComponentsTab()[mp_playerEntity->tab_index]->vec_components)
	{
		if (component->ID == Transform_ID)
		{
			transform = static_cast<TransformComponent*>(component);
		}
		if (component->ID == Velocity_ID)
		{
			velComponent = static_cast<VelocityComponent*>(component);
		}
	}


	// Mettez a jour la souris en passant le handle de la fenetre
	InputManager::UpdateMouse(GetActiveWindow());

	int deltaX = InputManager::GetMouseDeltaX();
	int deltaY = InputManager::GetMouseDeltaY();

	// Sensibilite de la souris
	const float sensitivity = 0.005f;

	// Mettre a jour la rotation de la camera en fonction du delta
	transform->m_transform.Rotation(0.0f, deltaY * sensitivity, deltaX * sensitivity);

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

	if (InputManager::GetKeyIsPressed(MK_RBUTTON))
	{
		AttackComponent* attack = nullptr;
		auto& playerComponents = mp_gameManager->GetEntityManager()->GetComponentsTab()[mp_playerEntity->tab_index]->vec_components;
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
			attack->attackRequested = true;
		}
	}

	if (transform)
	{
		transform->m_transform.ResetRoll();
	}

	if (InputManager::GetKeyDown('V'))
	{
		mp_camera->ChangeView();
	}
	if (InputManager::GetKeyIsPressed('N'))
	{
		mp_camera->SetTPS_Lock(true); 
	}
	else mp_camera->SetTPS_Lock(false);
}

void GameScene::OnClose()
{
	delete mp_playerEntity;
}
