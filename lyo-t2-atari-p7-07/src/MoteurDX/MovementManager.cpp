#include "pch.h"
#include "MovementManager.h"

#include "InitDirect3DApp.h"

void MovementManager::Initialize(InitDirect3DApp* gameManager)
{
	mp_gameManager = gameManager;
}

void MovementManager::Update()
{
	for (Entity* entity : mp_gameManager->GetEntityManager()->GetEntityTab())
	{
		if (!entity)
			continue;

		if (mp_gameManager->GetEntityManager()->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY))
		{
			TransformComponent* transform = nullptr;
			VelocityComponent* velocity = nullptr;


			for (auto* component : mp_gameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
			{
				if (component->ID == Transform_ID)
				{
					transform = static_cast<TransformComponent*>(component);
				}
				if (component->ID == Velocity_ID)
				{
					velocity = static_cast<VelocityComponent*>(component);
				}
			}
			if (transform != nullptr && velocity != nullptr)
			{
				Move(entity, velocity, transform);
			}
		}
	}
}

void MovementManager::SetVelocity(VelocityComponent* velComponent, float velFront, float velRight, float velUp)
{
	velComponent->vz = velFront;
	velComponent->vx = velRight;
	velComponent->vy = velUp;
}

void MovementManager::Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent)
{
	transformComponent->m_transform.Move(velComponent->vz, velComponent->vx, velComponent->vy);

	if (entity->id != 1) // Si ce n'est pas le joueur
	{
		if (transformComponent->m_transform.GetPositionX() > 145 || transformComponent->m_transform.GetPositionX() < -145
			|| transformComponent->m_transform.GetPositionY() > 145 || transformComponent->m_transform.GetPositionY() < -145
			|| transformComponent->m_transform.GetPositionZ() > 145 || transformComponent->m_transform.GetPositionZ() < -145)
		{
			mp_gameManager->GetEntityManager()->ToDestroy(entity);
		}
	}
	else // Si c'est le joueur
	{
		if (transformComponent->m_transform.GetPositionX() > 145)
		{
			transformComponent->m_transform.vPosition.x = 145;
		}
		else if (transformComponent->m_transform.GetPositionX() < -145)
		{
			transformComponent->m_transform.vPosition.x = -145;
		}
		if (transformComponent->m_transform.GetPositionY() > 145)
		{
			transformComponent->m_transform.vPosition.y = 145;
		}
		else if (transformComponent->m_transform.GetPositionY() < -145)
		{
			transformComponent->m_transform.vPosition.y = -145;
		}
		if (transformComponent->m_transform.GetPositionZ() > 145)
		{
			transformComponent->m_transform.vPosition.z = 145;
		}
		else if (transformComponent->m_transform.GetPositionZ() < -145)
		{
			transformComponent->m_transform.vPosition.z = -145;
		}
		SetVelocity(velComponent, 0, 0, 0);
	}
}
