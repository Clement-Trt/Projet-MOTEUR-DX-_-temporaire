#include "pch.h"

#include "SandBoxScene.h"
#include "EntityManager.h"
#include "InitDirect3DApp.h"
#include "InputManager.h"
#include "CameraSystem.h"
#include "AssetManager.h"

void SandBoxScene::CreateBoxBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, int health)
{
	Entity* newBoxBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newBoxBlock);
	mpEntityManager->AddComponent<MeshComponent>(newBoxBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newBoxBlock);

	if (health != 0)
		mpEntityManager->AddComponent<HealthComponent>(newBoxBlock);
	for (auto& comp : mpEntityManager->GetComponentToAddTab()[newBoxBlock->tab_index]->vec_components)
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
void SandBoxScene::CreateIceBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ)
{
	Entity* newIceBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newIceBlock);
	mpEntityManager->AddComponent<MeshComponent>(newIceBlock);
	mpEntityManager->AddComponent<HealthComponent>(newIceBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newIceBlock);

	for (auto& comp : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
			mesh->m_textureID = L"IceTexture";
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
			health->currentHealth = 1;
			health->maxHealth = 1;
		}
	}
}

void SandBoxScene::CreateMeteorBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, int health, bool rotation)
{
	Entity* newMeteorBlock = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(newMeteorBlock);
	mpEntityManager->AddComponent<MeshComponent>(newMeteorBlock);
	mpEntityManager->AddComponent<ColliderComponent>(newMeteorBlock);
	mpEntityManager->AddComponent<SceneObjectComponent>(newMeteorBlock);


	if (health != 0)
		for (auto& comp : mpEntityManager->GetComponentToAddTab()[newMeteorBlock->tab_index]->vec_components)
		{
			if (comp->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(comp);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
				mesh->m_textureID = L"MeteorTexture";
			}
			if (comp->ID == SceneObject_ID)
			{
				SceneObjectComponent* sceneObj = static_cast<SceneObjectComponent*>(comp);
				if (rotation)
				{
					float randRotX = -0.01 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.01 - -0.01);
					float randRotY = -0.01 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.01 - -0.01);
					float randRotZ = -0.01 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (0.01 - -0.01);

					sceneObj->speedRotX = randRotX;
					sceneObj->speedRotY = randRotY;
					sceneObj->speedRotZ = randRotZ;
				}
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

void SandBoxScene::CreateEnnemy(float posX, float posY, float posZ)
{

	Entity* ennemy = mpEntityManager->CreateEntity();
	mpEntityManager->AddComponent<TransformComponent>(ennemy);
	mpEntityManager->AddComponent<MeshComponent>(ennemy);
	mpEntityManager->AddComponent<ColliderComponent>(ennemy);
	mpEntityManager->AddComponent<EnnemyComponent>(ennemy);
	mpEntityManager->AddComponent<AttackComponent>(ennemy);
	mpEntityManager->AddComponent<HealthComponent>(ennemy);
	mpEntityManager->AddComponent<HighlightComponent>(ennemy);

	for (auto& comp : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[ennemy->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
			mesh->m_textureID = L"DroneTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(5.f, 5.f, 5.f);
			transform->m_transform.Move(posZ, posX, posY);
		}
		if (comp->ID == Health_ID)
		{
			HealthComponent* healthComp = static_cast<HealthComponent*>(comp);
			healthComp->maxHealth = healthComp->currentHealth = 10;
		}
		if (comp->ID == Highlight_ID)
		{
			HighlightComponent* highlight = static_cast<HighlightComponent*>(comp);
			highlight->isHighlighted = true;
			highlight->intensity = 3.0f;
		}
	}
}

void SandBoxScene::OnInitialize()
{
	// Sound
	std::string basePath = AssetManager::GetExecutablePath();

	std::string beamPath = basePath + "res\\bubblebeam.wav";
	std::string beamPlayerPath = basePath + "res\\BeamPlayer.wav";
	AssetManager::AddSound("beam", beamPath);
	AssetManager::AddSound("beamPlayer", beamPlayerPath);

	//// Music 															/!\/!\/!\/!\/!\/!\/!\/!\/!\
	//std::string electroPath = basePath + "res\\ElectroMusic.wav";
	//std::string albatorPath = basePath + "res\\AlbatorOST.wav";				ENLEVER ?
	//AssetManager::AddMusic("electro", electroPath);
	//AssetManager::AddMusic("albator", albatorPath);					/!\/!\/!\/!\/!\/!\/!\/!\/!\

	//AssetManager::GetMusic("electro").play();
	//AssetManager::GetMusic("electro").setVolume(15);


	// Create Entity 1 = player
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
		mpEntityManager->AddComponent<HighlightComponent>(entityPlayer);

		for (auto& component : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[entityPlayer->tab_index]->vec_components)
		{
			if (component->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(component);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
				mesh->m_textureID = L"PlayerTexture";
			}
			if (component->ID == Light_ID)
			{
				LightComponent* light = static_cast<LightComponent*>(component);
				light->type = LightType::Point;
				light->Position = { 0.0f, 0.0f, 0.0f };
				light->Color = { 5.0f, 5.0f, 5.0f };
				light->ConstantAtt = 5.f;
				light->LinearAtt = 0.09f;
				light->QuadraticAtt = 0.032f;
			}
			if (component->ID == Transform_ID)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(component);
				transform->m_transform.Scale(3.0f, 3.0f, 3.0f);
				transform->m_transform.vPosition = { 0.0f , 10.0f, 0.0f };
			}
			if (component->ID == Camera_ID)
			{
				CameraComponent* cam = static_cast<CameraComponent*>(component);
				cam->m_cameraTransform.Scale(1.0f, 1.0f, 1.0f);
				cam->m_cameraTransform.Rotation(0.0, 5.0, 0.0);
			}
			if (component->ID == Attack_ID)
			{
				AttackComponent* attack = static_cast<AttackComponent*>(component);
				attack->projectileTexture = L"PlayerBeamTexture";
				attack->attackCooldown = 0.1f;
				attack->damage = 2;

				attack->projectileSpeed = 5;
				attack->projectileSizeX = 0.2f;
				attack->projectileSizeY = 0.2f;
				attack->projectileSizeZ = 1.0f;
			}
			if (component->ID == Highlight_ID)
			{
				HighlightComponent* highlight = static_cast<HighlightComponent*>(component);
				highlight->isHighlighted = true;
				highlight->intensity = 3.0f;
			}
		}
		mp_playerEntity = entityPlayer;
	}

	// Body
	Entity* playerBody = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<PlayerComponent>(playerBody);
	mpEntityManager->AddComponent<TransformComponent>(playerBody);
	mpEntityManager->AddComponent<MeshComponent>(playerBody);
	mpEntityManager->AddComponent<VelocityComponent>(playerBody);
	mpEntityManager->AddComponent<LightComponent>(playerBody);
	mpEntityManager->AddComponent<HighlightComponent>(playerBody);
	for (auto& component : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[playerBody->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
			mesh->m_textureID = L"PlayerTexture";
		}
		if (component->ID == Light_ID)
		{
			LightComponent* light = static_cast<LightComponent*>(component);
			light->type = LightType::Point;
			light->Position = { 0.0f, 0.0f, 0.0f };
			light->Color = { 5.0f, 5.0f, 5.0f };
			light->ConstantAtt = 5.f;
			light->LinearAtt = 0.09f;
			light->QuadraticAtt = 0.032f;
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(3.0f, 7.0f, 3.0f);
			transform->m_transform.vPosition = { 0.0f , 3.5f, 0.0f };
		}
		if (component->ID == Highlight_ID)
		{
			HighlightComponent* highlight = static_cast<HighlightComponent*>(component);
			highlight->isHighlighted = true;
			highlight->intensity = 3.0f;
		}
	}
	mp_playerBody = playerBody;

	// Create Floor
	{
		Entity* floor = mpEntityManager->CreateEntity();

		mpEntityManager->AddComponent<TransformComponent>(floor);
		mpEntityManager->AddComponent<MeshComponent>(floor);

		for (auto& component : mp_gameManager->GetEntityManager()->GetComponentToAddTab()[floor->tab_index]->vec_components)
		{
			if (component->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(component);
				mesh->m_cubeMesh = mp_gameManager->GetFactory()->CreateCube();
				mesh->m_textureID = L"WallTexture";
			}
			if (component->ID == Transform_ID)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(component);
				transform->m_transform.Scale(500.f, 0.0f, 500.f);
			}
		}
	}

	// Create Ice Wall
	for (int i = 1; i < 10; i++)
	{
		for (int j = 1; j < 10; j++)
		{
			float posX = 10 * i - 50;
			float posY = 10 * j;
			CreateIceBlock(8, 8, 8, posX, posY, -60);
		}
	}

	mp_camera->SetTPS();

	CreateBoxBlock(4, 4, 4, -12, 10, 60, 0);
	CreateIceBlock(4, 4, 4, -6, 10, 60);
	CreateEnnemy(0, 10, 60);
	CreateMeteorBlock(4, 4, 4, 6, 10, 60, 30, false);
	CreateMeteorBlock(4, 4, 4, 12, 10, 60, 30, true);
}

void SandBoxScene::OnUpdate()
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

	if (InputManager::GetKeyIsPressed('D')) /*transform->m_transform.vPosition.x += 1.5f;*/ velComponent->vx = 0.75f;
	if (InputManager::GetKeyIsPressed('Q')) /*transform->m_transform.vPosition.x -= 1.5f;*/ velComponent->vx = -0.75f;

	if (InputManager::GetKeyIsPressed('Z')) /*transform->m_transform.vPosition.z += 1.5f;*/ velComponent->vz = 0.75f;
	if (InputManager::GetKeyIsPressed('S')) /*transform->m_transform.vPosition.z -= 1.5f;*/ velComponent->vz = -0.75f;

	if (InputManager::GetKeyIsPressed(VK_SPACE)) transform->m_transform.vPosition.y += 2.0f;
	//if (InputManager::GetKeyIsPressed('E')) velComponent->vy = -1.5f;


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
		transform->m_transform.vPosition.y -= 0.75;
		if (transform->m_transform.GetPositionY() < 10)
		{
			transform->m_transform.vPosition.y = 10;
		}
		transform->m_transform.ResetRoll();
	}

	TransformComponent* transform2 = nullptr;
	//VelocityComponent* velComponent2 = nullptr;

	for (auto* component : mpEntityManager->GetComponentsTab()[mp_playerBody->tab_index]->vec_components)
	{
		if (component->ID == Transform_ID)
		{
			transform2 = static_cast<TransformComponent*>(component);
		}/*
		if (component->ID == Velocity_ID)
		{
			velComponent2 = static_cast<VelocityComponent*>(component);
		}*/
	}
	if (transform2)
	{
		transform2->m_transform.vPosition.x = transform->m_transform.vPosition.x;
		transform2->m_transform.vPosition.y = transform->m_transform.vPosition.y - ((transform2->m_transform.vScale.y - transform->m_transform.vScale.y)) - 1;
		transform2->m_transform.vPosition.z = transform->m_transform.vPosition.z;
	}

	if (InputManager::GetKeyDown('A'))
	{
		mp_camera->ChangeView();
	}
	if (InputManager::GetKeyIsPressed('X'))
	{
		mp_camera->SetTPS_Lock(true);
	}
	else mp_camera->SetTPS_Lock(false);

}

void SandBoxScene::OnClose()
{
	Close();
	delete mp_playerEntity;
}
