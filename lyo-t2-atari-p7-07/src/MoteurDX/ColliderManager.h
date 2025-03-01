#pragma once

#include "EntityManager.h"
#include "ParticleManager.h"

using namespace DirectX;

struct AABB {
	XMFLOAT3 Min;
	XMFLOAT3 Max;
};

class ColliderManager
{
public:
	void InitCollider(EntityManager* entityManager, ParticleManager* particleManager);

	void UpdateCollider();

	bool AABBIntersect(const TransformComponent& a, const TransformComponent& b);

	XMVECTOR ResolveAABBCollision(const TransformComponent& a, const TransformComponent& b);

private:

	EntityManager* m_entityManager;
	ParticleManager* m_particleManager;
};


