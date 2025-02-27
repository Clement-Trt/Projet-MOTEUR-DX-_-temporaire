#pragma once

#include "EntityManager.h"

using namespace DirectX;

struct AABB {
	XMFLOAT3 Min;
	XMFLOAT3 Max;
};

class ColliderManager
{
public:
	void InitCollider(EntityManager* entityManager);

	void UpdateCollider();

	bool AABBIntersect(const TransformComponent& a, const TransformComponent& b);
	XMVECTOR ResolveAABBCollision(const TransformComponent& a, const TransformComponent& b);
	bool AABBIntersect(TransformComponent& a, TransformComponent& b);

private:

	EntityManager* m_entityManager;
};


