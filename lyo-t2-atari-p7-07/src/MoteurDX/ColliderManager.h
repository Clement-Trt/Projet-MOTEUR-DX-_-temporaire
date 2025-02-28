#pragma once

#include "EntityManager.h"

class ColliderManager
{
public:
	void InitCollider(EntityManager* entityManager);

	void UpdateCollider();

	bool AABBIntersect(TransformComponent& a, TransformComponent& b);

private:

	EntityManager* m_entityManager;
};

