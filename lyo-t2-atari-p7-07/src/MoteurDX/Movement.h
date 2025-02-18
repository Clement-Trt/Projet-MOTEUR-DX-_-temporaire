#pragma once

#include <memory>

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

	void SetVelocity(Entity* entity, float velFront, float velRight, float velUp); 
};

