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
		if (transformComponent->m_transform.GetPositionX() > 2490 || transformComponent->m_transform.GetPositionX() < -2490
			|| transformComponent->m_transform.GetPositionY() > 2490 || transformComponent->m_transform.GetPositionY() < -2490
			|| transformComponent->m_transform.GetPositionZ() > 2490 || transformComponent->m_transform.GetPositionZ() < -2490)
		{
			mGM->GetEntityManager()->ToDestroy(entity);
		}
	}
	else
	{
		if (transformComponent->m_transform.GetPositionX() > 2490)
		{
			transformComponent->m_transform.vPosition.x = 2490;
			//transformComponent->m_transform.Move(0, -velComponent->vx, 0);
		}
		else if (transformComponent->m_transform.GetPositionX() < -2490)
		{
			transformComponent->m_transform.vPosition.x = -2490;
			//transformComponent->m_transform.Move(0, -velComponent->vx, 0);
		}
		if (transformComponent->m_transform.GetPositionY() > 2490)
		{
			transformComponent->m_transform.vPosition.y = 2490;
			//transformComponent->m_transform.Move(0, 0, -velComponent->vy);
		}
		else if (transformComponent->m_transform.GetPositionY() < -2490)
		{
			transformComponent->m_transform.vPosition.y = -2490;
			//transformComponent->m_transform.Move(0, 0, -velComponent->vy);
		}
		if (transformComponent->m_transform.GetPositionZ() > 2490)
		{
			transformComponent->m_transform.vPosition.z = 2490;
			//transformComponent->m_transform.Move(-velComponent->vz, 0, 0);
		}
		else if (transformComponent->m_transform.GetPositionZ() < -2490)
		{
			transformComponent->m_transform.vPosition.z = -2490;
			//transformComponent->m_transform.Move(-velComponent->vz, 0, 0);
		}
	}
}
