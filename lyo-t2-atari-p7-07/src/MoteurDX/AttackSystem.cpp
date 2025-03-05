#include "pch.h"
#include "AttackSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h" // Pour AttackComponent
#include "AssetManager.h"

void AttackSystem::Initialize(InitDirect3DApp* gameManager)
{
    m_gameManager = gameManager;
	
	// Sound
	std::string basePath = AssetManager::GetExecutablePath();

	std::string beamPath = basePath + "res\\bubblebeam.wav";
	std::string beamPlayerPath = basePath + "res\\BeamPlayer.wav";
	std::string beamEnnemyPath = basePath + "res\\BeamEnnemy.wav";
	AssetManager::AddSound("beam", beamPath); 
	AssetManager::AddSound("beamPlayer", beamPlayerPath);
	AssetManager::AddSound("beamEnnemy", beamEnnemyPath);
}

void AttackSystem::Update(float deltaTime)
{
    EntityManager* entityManager = m_gameManager->GetEntityManager();
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

		if (entityManager->HasComponent(entity, COMPONENT_ATTACK))
		{
			AttackComponent* attack = nullptr;
			TransformComponent* entityTransform = nullptr;
			PlayerComponent* player = nullptr;
			EnnemyComponent* ennemy = nullptr;
			auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
			for (auto* comp : compTab)
			{
				if (comp->ID == Attack_ID)
				{
					attack = static_cast<AttackComponent*>(comp);
				}
				if (comp->ID == Transform_ID)
				{
					entityTransform = static_cast<TransformComponent*>(comp);
				}
				if (comp->ID == Player_ID)
				{
					player = static_cast<PlayerComponent*>(comp);
				}
				if (comp->ID == Ennemy_ID)
				{
					ennemy = static_cast<EnnemyComponent*>(comp);
				}
			}
			if (attack)
			{
				// Toujours incr�menter le temps �coul�E
				attack->timeSinceLastAttack += deltaTime;

				// Si une attaque est demand�e et que le cooldown est respect�E
				if (attack->attackRequested && attack->timeSinceLastAttack >= attack->attackCooldown)
				{

					//MessageBox(0, L"Attaque du joueur !", 0, 0);

					// Projectile
					Entity* newBullet = entityManager->CreateEntity();

					entityManager->AddComponent<TransformComponent>(newBullet);
					entityManager->AddComponent<MeshComponent>(newBullet);
					entityManager->AddComponent<VelocityComponent>(newBullet);
					entityManager->AddComponent<ColliderComponent>(newBullet);
					entityManager->AddComponent<AttackComponent>(newBullet);
					entityManager->AddComponent<LifeTimeComponent>(newBullet);

					TransformComponent* transform = nullptr;
					MeshComponent* mesh = nullptr;
					VelocityComponent* vel = nullptr;
					ColliderComponent* collider = nullptr;
					AttackComponent* bulletProperty = nullptr;
					LifeTimeComponent* lifetime = nullptr;

					for (auto* component : entityManager->GetComponentToAddTab()[newBullet->tab_index]->vec_components)
					{
						if (component->ID == Mesh_ID)
						{
							mesh = static_cast<MeshComponent*>(component);
							mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
							mesh->textureID = attack->projectileTexture; // On assigne la texture
						}
						if (component->ID == Transform_ID)
						{
							transform = static_cast<TransformComponent*>(component);
							transform->m_transform = entityTransform->m_transform;
							transform->m_transform.Move(2.0f, 0, 0);
							transform->m_transform.Scale(attack->projectileSizeX, attack->projectileSizeY, attack->projectileSizeZ);
						}
						if (component->ID == Velocity_ID)
						{
							vel = static_cast<VelocityComponent*>(component);
							vel->vx = 0.0f;
							vel->vy = 0.0f;
							vel->vz = attack->projectileSpeed;
						}
						if (component->ID == Collider_ID)
						{
							collider = static_cast<ColliderComponent*>(component);
							collider->m_isDestructable = true;
						}
						if (component->ID == Attack_ID)
						{
							bulletProperty = static_cast<AttackComponent*>(component);
							bulletProperty->damage = attack->damage;
						}
						if (component->ID == LifeTime_ID)
						{
							lifetime = static_cast<LifeTimeComponent*>(component);
							lifetime->lifeTime = 1.5f;
						}

					}
					if (player != nullptr)
					{
						AssetManager::PlayLocalSound("beamPlayer");
					}
					if (ennemy != nullptr)
					{
						AssetManager::PlayLocalSound("beamEnnemy");
					}
					// Reinitialiser le cooldown et le flag d'attaque
					attack->timeSinceLastAttack = 0.0f;
					attack->attackRequested = false;
				}
			}
		}
	}
}
