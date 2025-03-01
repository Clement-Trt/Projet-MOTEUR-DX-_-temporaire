#pragma once

#include "EntityManager.h"
#include "InitDirect3DApp.h"

class ParticleManager
{
public:
	ParticleManager();

	void Initialize(InitDirect3DApp* app);

	void Update();

	void CreateDefaultBlock(float startPosX, float startPosY, float startPosZ, float size, float speedX, float speedY, float speedZ);

	void MakeEffect(float startPosX, float startPosY, float startPosZ, int _minNbPart, int _maxNbPart, int _minSpeed, int _maxSpeed, int minSize, int maxSize);

	void CreateParticle(float size, float speed);

	void Explosion(float startPosX, float startPosY, float startPosZ);

private:

	EntityManager* m_entityManager;
	InitDirect3DApp* m_gameManager;

};

