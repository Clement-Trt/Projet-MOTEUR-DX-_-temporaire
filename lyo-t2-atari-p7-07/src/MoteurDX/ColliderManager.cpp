#include "pch.h"
#include "ColliderManager.h"

void ColliderManager::InitCollider(EntityManager* entityManager)
{
	m_entityManager = entityManager;
}

void ColliderManager::UpdateCollider()
{
	// Collisions
	for (auto& entity1 : m_entityManager->GetEntityTab())
	{
		if (entity1 == nullptr)
			continue;

		// Transform Component
		TransformComponent* transform1 = nullptr;
		for (auto* component : m_entityManager->GetComponentsTab()[entity1->tab_index]->vec_components)
		{
			if (component->ID == Transform_ID)
			{
				transform1 = static_cast<TransformComponent*>(component);
			}
		}
		if (!transform1)
			continue;

		// Collider Component
		ColliderComponent* collider1 = nullptr;
		for (auto* component : m_entityManager->GetComponentsTab()[entity1->tab_index]->vec_components)
		{
			if (component->ID == Collider_ID)
			{
				collider1 = static_cast<ColliderComponent*>(component);
			}
		}
		if (!collider1)
			continue;

		for (uint32_t entity2 = entity1->tab_index + 1; entity2 < 64000; entity2++)
		{
			if (!m_entityManager->GetEntityTab()[entity2])
				continue;

			TransformComponent* transform2 = nullptr;
			for (auto* component : m_entityManager->GetComponentsTab()[entity2]->vec_components)
			{
				if (component->ID == Transform_ID)
				{
					transform2 = static_cast<TransformComponent*>(component);
				}
			}
			if (!transform2)
				continue;

			// Collider Component
			ColliderComponent* collider2 = nullptr;
			for (auto* component : m_entityManager->GetComponentsTab()[entity2]->vec_components)
			{
				if (component->ID == Collider_ID)
				{
					collider2 = static_cast<ColliderComponent*>(component);
				}
			}
			if (!collider2)
				continue;

			// Test de collision
			if (AABBIntersect(*transform1, *transform2))
			{
				// Ici, vous pouvez ajouter le traitement neessaire en cas de collision
				collider1->m_isColliding = true;
				collider2->m_isColliding = true;
				OutputDebugString(L"Collision !\n");
			}
			else
			{
				collider1->m_isColliding = false;
				collider2->m_isColliding = false;
			}
		}
	}
}

bool ColliderManager::AABBIntersect(TransformComponent& a, TransformComponent& b)
{
	// Calcul des bornes pour l'objet A
	float aMinX = a.m_transform.GetPositionX() - a.m_transform.GetScale().x;
	float aMaxX = a.m_transform.GetPositionX() + a.m_transform.GetScale().x;
	float aMinY = a.m_transform.GetPositionY() - a.m_transform.GetScale().y;
	float aMaxY = a.m_transform.GetPositionY() + a.m_transform.GetScale().y;
	float aMinZ = a.m_transform.GetPositionZ() - a.m_transform.GetScale().z;
	float aMaxZ = a.m_transform.GetPositionZ() + a.m_transform.GetScale().z;

	// Calcul des bornes pour l'objet B
	float bMinX = b.m_transform.GetPositionX() - b.m_transform.GetScale().x;
	float bMaxX = b.m_transform.GetPositionX() + b.m_transform.GetScale().x;
	float bMinY = b.m_transform.GetPositionY() - b.m_transform.GetScale().y;
	float bMaxY = b.m_transform.GetPositionY() + b.m_transform.GetScale().y;
	float bMinZ = b.m_transform.GetPositionZ() - b.m_transform.GetScale().z;
	float bMaxZ = b.m_transform.GetPositionZ() + b.m_transform.GetScale().z;

	// Vérification du chevauchement sur chaque axe
	if (aMaxX < bMinX || aMinX > bMaxX)
		return false;
	if (aMaxY < bMinY || aMinY > bMaxY)
		return false;
	if (aMaxZ < bMinZ || aMinZ > bMaxZ)
		return false;

	return true;
}
