#include "pch.h"

#include "Movement.h"
//#include "GameManager.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"

void Movement::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void Movement::SetVelocity(/*Entity* entity, */VelocityComponent* velComponent, float velFront, float velRight, float velUp)
{
	velComponent->vz = velFront;
	velComponent->vx = velRight;
	velComponent->vy = velUp;
}

void Movement::Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent)
{
	transformComponent->m_transform.Move(velComponent->vz, velComponent->vx,velComponent->vy);
	if (entity->id != 1)
	{
		if (transformComponent->m_transform.GetPositionX() > 70 || transformComponent->m_transform.GetPositionX() < -70
			|| transformComponent->m_transform.GetPositionY() > 70 || transformComponent->m_transform.GetPositionY() < -70
			|| transformComponent->m_transform.GetPositionZ() > 70 || transformComponent->m_transform.GetPositionZ() < -70)
		{
			mGM->GetEntityManager()->ToDestroy(entity);
		}
	}
	else
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
	}
}
