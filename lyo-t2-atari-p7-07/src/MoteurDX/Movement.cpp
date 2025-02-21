#include "pch.h"

#include "Movement.h"
//#include "GameManager.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"

void Movement::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void Movement::SetVelocity(VelocityComponent* velComponent, float velFront, float velRight, float velUp)
{
	velComponent->vz = velFront;
	velComponent->vx = velRight;
	velComponent->vy = velUp;

	/*VelocityComponent* vel = static_cast<VelocityComponent*>(mGM->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->tab_components[Velocity_index]);
	vel->vz = velFront;
	vel->vx = velRight;
	vel->vy = velUp;*/
}
