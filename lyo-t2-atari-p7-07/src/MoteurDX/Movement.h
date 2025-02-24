#pragma once

#include "Components.h"
#include "EntityManager.h"

//class EntityManager;
class GameManager;
class InitDirect3DApp;
//class EntityManager;

class Movement
{
protected:
	//std::unique_ptr<EntityManager> mEM = std::make_unique<EntityManager>();

	InitDirect3DApp* mGM;
	//EntityManager* mEM;

	//VelocityComponent* mVc;

public:

	void Initialize(InitDirect3DApp* gameManager);

	//void SetVelocity(VelocityComponent* velComponent, float velFront, float velRight, float velUp);
	void SetVelocity(Entity* entity, VelocityComponent* velComponent, float velFront, float velRight, float velUp);

	//void Move(VelocityComponent* velComponent, TransformComponent* transformComponent);
	void Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent);
};

