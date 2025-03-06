#include "pch.h"
#include "MovementManager.h"

#include "InitDirect3DApp.h"

void MovementManager::Initialize(InitDirect3DApp* gameManager)
{
	mp_gameManager = gameManager;
	m_limitPosMin = -220; //-145
	m_limitPosMax = 220; //145

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
		if (transformComponent->m_transform.GetPositionX() > m_limitPosMax || transformComponent->m_transform.GetPositionX() < m_limitPosMin
			|| transformComponent->m_transform.GetPositionY() > m_limitPosMax || transformComponent->m_transform.GetPositionY() < m_limitPosMin
			|| transformComponent->m_transform.GetPositionZ() > m_limitPosMax || transformComponent->m_transform.GetPositionZ() < m_limitPosMin)
		{
			mp_gameManager->GetEntityManager()->ToDestroy(entity);
		}
	}
	else // Si c'est le joueur
	{
		if (transformComponent->m_transform.GetPositionX() > m_limitPosMax)
		{
			transformComponent->m_transform.vPosition.x = m_limitPosMax;
		}
		else if (transformComponent->m_transform.GetPositionX() < m_limitPosMin)
		{
			transformComponent->m_transform.vPosition.x = m_limitPosMin;
		}
		if (transformComponent->m_transform.GetPositionY() > m_limitPosMax)
		{
			transformComponent->m_transform.vPosition.y = m_limitPosMax;
		}
		else if (transformComponent->m_transform.GetPositionY() < m_limitPosMin)
		{
			transformComponent->m_transform.vPosition.y = m_limitPosMin;
		}
		if (transformComponent->m_transform.GetPositionZ() > m_limitPosMax)
		{
			transformComponent->m_transform.vPosition.z = m_limitPosMax;
		}
		else if (transformComponent->m_transform.GetPositionZ() < m_limitPosMin)
		{
			transformComponent->m_transform.vPosition.z = m_limitPosMin;
		}
		SetVelocity(velComponent, 0, 0, 0);
	}
}
