#pragma once

#include "Components.h"
#include "EntityManager.h"

class GameManager;
class InitDirect3DApp;

class MovementManager
{
protected:

	InitDirect3DApp* mp_gameManager;

public:

	void Initialize(InitDirect3DApp* gameManager);

	void Update();

	void SetVelocity(VelocityComponent* velComponent, float velFront, float velRight, float velUp);

	void Move(Entity* entity, VelocityComponent* velComponent, TransformComponent* transformComponent);
};

