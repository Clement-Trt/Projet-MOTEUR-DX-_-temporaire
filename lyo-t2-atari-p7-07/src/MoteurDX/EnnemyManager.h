#pragma once

#include "EntityManager.h"
#include "InitDirect3DApp.h"

class EnnemyManager
{
public:
	EnnemyManager();

	void InitEnnemyManager(EntityManager* entityManager, InitDirect3DApp* app);

	void Update();

private:

	EntityManager* m_entityManager;
	InitDirect3DApp* m_gameManager;
	TransformComponent* m_playerTransform;
	Entity* m_player;
};

