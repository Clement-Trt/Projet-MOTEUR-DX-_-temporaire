#include "pch.h"

#include "GameScene.h"
#include "EntityManager.h"
#include "InitDirect3DApp.h"
#include "InputManager.h"
#include "CameraSystem.h"
#include "AssetManager.h"

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
	mpEntityManager->AddComponent<SceneObjectComponent>(newIceBlock);
	SceneObjectComponent* sceneObj = nullptr;

	float randRotX = -0.001 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.001 - -0.001);
	float randRotY = -0.001 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.001 - -0.001);
	float randRotZ = -0.001 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.001 - -0.001);

	if (health != 0)
		for (auto& comp : mpEntityManager->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
		{
			if (comp->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(comp);
				mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
				mesh->textureID = L"MeteorTexture";
			}
			if (comp->ID == SceneObject_ID)
			{
				sceneObj = static_cast<SceneObjectComponent*>(comp);
				sceneObj->speedRotX = randRotX;
				sceneObj->speedRotY = randRotY;
				sceneObj->speedRotZ = randRotZ;
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
	// Music
	std::string basePath = AssetManager::GetExecutablePath();

	std::string electroPath = basePath + "res\\ElectroMusic.wav";
	std::string albatorPath = basePath + "res\\AlbatorOST.wav";
	AssetManager::AddMusic("electro", electroPath);
	AssetManager::AddMusic("albator", albatorPath);

	AssetManager::GetMusic("electro").play();
	AssetManager::GetMusic("electro").setVolume(20);

	// Entity 1 = player
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
				attack->projectileTexture = L"PlayerBeamTexture";
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
				transform->m_transform.Scale(700, 700, 700);
				transform->m_transform.Move(0, 0, 0);
			}
		}
	}

	// Meteors
	int nbMeteor = 20;
	int nbMeteorInScene = 0;

	while (nbMeteorInScene < nbMeteor)
	{
		float randSizeX = 10 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (90 - 10);
		float randSizeY = 10 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (90 - 10);
		float randSizeZ = 10 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (90 - 10);

		float randPosX = -300 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (300 - -300);
		float randPosY = -300 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (300 - -300);
		float randPosZ = -300 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (300 - -300);

		CreateWallBlock(randSizeX, randSizeY, randSizeZ, randPosX, randPosY, randPosZ, 10);
		nbMeteorInScene++;
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

	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
			continue; // L'entite est nulle, on passe a la suivante

		// Pointeurs pour les composants
		TransformComponent* objTransform = nullptr;
		SceneObjectComponent* sceneObj = nullptr;

		// On parcourt les composants de l'entite
		for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
		{
			if (component->ID == SceneObject_ID)
				sceneObj = static_cast<SceneObjectComponent*>(component);

			if (component->ID == Transform_ID)
				objTransform = static_cast<TransformComponent*>(component);
		}

		// Si les deux composants sont trouves, on applique la rotation
		if (sceneObj && objTransform)
		{
			objTransform->m_transform.Rotation(sceneObj->speedRotX, sceneObj->speedRotY, sceneObj->speedRotZ);
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

	// Si la touche 'P' est presse, on demande une attaque du joueur sur l'IceBlock
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
