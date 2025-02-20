#include "pch.h"

#include "Movement.h"
//#include "GameManager.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"

void Movement::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void Movement::SetVelocity(Entity* entity, float velFront, float velRight, float velUp)
{
	for (auto& component : mGM->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->vec_components)
	{
		if (component->ID == Velocity_ID)
		{
			VelocityComponent* vel = static_cast<VelocityComponent*>(component);
			vel->vz = velFront;
			vel->vx = velRight;
			vel->vy = velUp;
		}
	}
	/*VelocityComponent* vel = static_cast<VelocityComponent*>(mGM->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->tab_components[Velocity_index]);
	vel->vz = velFront;
	vel->vx = velRight;
	vel->vy = velUp;*/
}
