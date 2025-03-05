#include "pch.h"
#include "AttackSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h"

void AttackSystem::Initialize(InitDirect3DApp* gameManager)
{
    m_gameManager = gameManager;
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
				attack->timeSinceLastAttack += deltaTime;

				if (attack->attackRequested && attack->timeSinceLastAttack >= attack->attackCooldown)
				{
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
					LifeTimeComponent* p_lifeTime = nullptr;

					for (auto* component : entityManager->GetComponentToAddTab()[newBullet->tab_index]->vec_components)
					{
						if (component->ID == Mesh_ID)
						{
							mesh = static_cast<MeshComponent*>(component);
							mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
							mesh->m_textureID = attack->projectileTexture;
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
							p_lifeTime = static_cast<LifeTimeComponent*>(component);
							p_lifeTime->m_lifeTime = 1.5f;
						}
					}
					attack->timeSinceLastAttack = 0.0f;
					attack->attackRequested = false;
				}
			}
		}
	}
}
