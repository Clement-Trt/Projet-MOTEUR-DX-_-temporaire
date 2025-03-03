#include "pch.h"
#include "AttackSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h" // Pour AttackComponent

void AttackSystem::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void AttackSystem::Update(float deltaTime)
{
	EntityManager* entityManager = mGM->GetEntityManager();
	for (Entity* entity : entityManager->GetEntityTab())
	{
		if (!entity)
			continue;

		if (entityManager->HasComponent(entity, COMPONENT_ATTACK))
		{
			AttackComponent* attack = nullptr;
			TransformComponent* entityTransform = nullptr;
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
			}
			if (attack)
			{
				// Toujours incrémenter le temps écoulE
				attack->timeSinceLastAttack += deltaTime;

				// Si une attaque est demandée et que le cooldown est respectE
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
							mesh->m_cubeMesh = mGM->GetFactory()->CreateCube();
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
					// Reinitialiser le cooldown et le flag d'attaque
					attack->timeSinceLastAttack = 0.0f;
					attack->attackRequested = false;
				}
			}
		}
	}
}
