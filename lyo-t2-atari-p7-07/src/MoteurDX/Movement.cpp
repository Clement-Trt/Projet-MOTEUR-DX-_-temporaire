#include "pch.h"

#include "Movement.h"
//#include "GameManager.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"

void Movement::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

//void Movement::SetVelocity(VelocityComponent* velComponent, float velFront, float velRight, float velUp)
//{
//	velComponent->vz = velFront;
//	velComponent->vx = velRight;
//	velComponent->vy = velUp;
//
//	/*VelocityComponent* vel = static_cast<VelocityComponent*>(mGM->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->tab_components[Velocity_index]);
//	velComponent->vz = velFront;
//	velComponent->vx = velRight;
//	velComponent->vy = velUp;*/
//}
void Movement::SetVelocity(Entity* entity, VelocityComponent* velComponent, float velFront, float velRight, float velUp)
{
	velComponent->vz = velFront;
	velComponent->vx = velRight;
	velComponent->vy = velUp;
}

//void Movement::Move(VelocityComponent* velComponent, TransformComponent* transformComponent)
//{
//	transformComponent->m_transform.Move(velComponent->vx, velComponent->vx,velComponent->vy);
//
//	if (transformComponent->m_transform.GetPositionX() > 100 || transformComponent->m_transform.GetPositionX() < -100)
//	{
//		
//	}
//}

void Movement::Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent)
{
	/*TransformComponent* transform = nullptr;
	VelocityComponent* vel = nullptr;


	for (auto* component : mGM->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
	{
		if (component->ID == Transform_ID)
		{
			transform = static_cast<TransformComponent*>(component);
		}
		if (component->ID == Velocity_ID)
		{
			vel = static_cast<VelocityComponent*>(component);
		}
	}*/
	//auto& transformable = transformComponent->m_transform;

	transformComponent->m_transform.Move(velComponent->vz, velComponent->vx,velComponent->vy);

	if (transformComponent->m_transform.GetPositionX() > 100 || transformComponent->m_transform.GetPositionX() < -100
		|| transformComponent->m_transform.GetPositionY() > 100 || transformComponent->m_transform.GetPositionY() < -100
		|| transformComponent->m_transform.GetPositionZ() > 100 || transformComponent->m_transform.GetPositionZ() < -100)
	{
		mGM->GetEntityManager()->ToDestroy(entity);
	}
}
