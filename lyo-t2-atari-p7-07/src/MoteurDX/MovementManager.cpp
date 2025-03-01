#include "pch.h"
#include "MovementManager.h"

#include "InitDirect3DApp.h"

void MovementManager::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void MovementManager::Update()
{
	for (Entity* entity : mGM->GetEntityManager()->GetEntityTab())
	{
		if (!entity)
			continue;

		if (mGM->GetEntityManager()->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY))
		{
			TransformComponent* transform = nullptr;
			VelocityComponent* velocity = nullptr;


			for (auto* component : mGM->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
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

void MovementManager::SetVelocity(/*Entity* entity, */VelocityComponent* velComponent, float velFront, float velRight, float velUp)
{
	velComponent->vz = velFront;
	velComponent->vx = velRight;
	velComponent->vy = velUp;
}

void MovementManager::Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent)
{
	transformComponent->m_transform.Move(velComponent->vz, velComponent->vx, velComponent->vy);

	if (entity->id != 1 && entity->id != 2) // Si ce n'est pas le joueur
	{
		if (transformComponent->m_transform.GetPositionX() > 70 || transformComponent->m_transform.GetPositionX() < -70
			|| transformComponent->m_transform.GetPositionY() > 70 || transformComponent->m_transform.GetPositionY() < -70
			|| transformComponent->m_transform.GetPositionZ() > 70 || transformComponent->m_transform.GetPositionZ() < -70)
		{
			mGM->GetEntityManager()->ToDestroy(entity);
		}
	}
	else // Si c'est le joueur
	{
		if (transformComponent->m_transform.GetPositionX() > 70)
		{
			transformComponent->m_transform.vPosition.x = 70;
		}
		else if (transformComponent->m_transform.GetPositionX() < -70)
		{
			transformComponent->m_transform.vPosition.x = -70;
		}
		if (transformComponent->m_transform.GetPositionY() > 70)
		{
			transformComponent->m_transform.vPosition.y = 70;
		}
		else if (transformComponent->m_transform.GetPositionY() < -70)
		{
			transformComponent->m_transform.vPosition.y = -70;
		}
		if (transformComponent->m_transform.GetPositionZ() > 70)
		{
			transformComponent->m_transform.vPosition.z = 70;
		}
		else if (transformComponent->m_transform.GetPositionZ() < -70)
		{
			transformComponent->m_transform.vPosition.z = -70;
		}
		SetVelocity(velComponent, 0, 0, 0);
	}
}
