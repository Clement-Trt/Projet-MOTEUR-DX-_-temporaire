#pragma once

#include "EntityManager.h"

class ColliderManager
{
public:
	void InitCollider(EntityManager* entityManager);

	void UpdateCollider();

	bool AABBIntersect(const TransformComponent& a, const TransformComponent& b);

private:

	EntityManager* m_entityManager;
};

