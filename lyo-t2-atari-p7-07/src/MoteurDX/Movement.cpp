#include "pch.h"

#include "Movement.h"
#include "GameManager.h"
//#include "EntityManager.h"

void Movement::Initialize(GameManager* gameManager)
{
	mGM = gameManager;
}

void Movement::SetVelocity(Entity* entity, float velFront, float velRight, float velUp)
{
	VelocityComponent* vel = static_cast<VelocityComponent*>(mGM->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->tab_components[Velocity_index]);
	vel->vz = velFront;
	vel->vx = velRight;
	vel->vy = velUp;
}
