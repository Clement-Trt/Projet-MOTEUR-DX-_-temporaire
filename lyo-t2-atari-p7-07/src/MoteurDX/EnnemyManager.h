#pragma once

#include "EntityManager.h"
#include "InitDirect3DApp.h"

class EnnemyManager
{
public:
	EnnemyManager();

	void InitEnnemyManager(EntityManager* entityManager, InitDirect3DApp* app);

	void Update();

	void EnnemyAttackSystem();

	void SpawnEnnemy(float posX, float posY, float posZ);
	void SpawnEnnemyBoss(float posX, float posY, float posZ);

	void WaveSystem();

	int GetNbEnnemy() { return m_nbEnnemy; };
	void SetNbEnnemy(int value) { m_nbEnnemy = value; };

private:

	EntityManager* m_entityManager;
	InitDirect3DApp* m_gameManager;
	TransformComponent* m_playerTransform;
	Entity* m_player;

	bool m_startNextWave;

	int m_waveNb;
	int m_nbEnnemy, m_nbEnnemyToSpawn;
};

