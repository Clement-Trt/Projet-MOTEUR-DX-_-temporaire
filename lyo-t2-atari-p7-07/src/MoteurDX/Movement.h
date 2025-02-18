#pragma once

#include <memory>

#include "Components.h"
#include "EntityManager.h"

//class EntityManager;
class GameManager;
//class EntityManager;

class Movement
{
protected:
	//std::unique_ptr<EntityManager> mEM = std::make_unique<EntityManager>();

	GameManager* mGM;
	//EntityManager* mEM;

	//VelocityComponent* mVc;

public:

	void Initialize(GameManager* gameManager);

	void SetVelocity(Entity* entity, float velFront, float velRight, float velUp); 
};

