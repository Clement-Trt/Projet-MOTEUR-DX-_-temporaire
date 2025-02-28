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
			continue;// Msg d'erreur ?

		// Transform & Collider Components
		TransformComponent* transform1 = nullptr;
		ColliderComponent* collider1 = nullptr;
		HealthComponent* health1 = nullptr;
		AttackComponent* attack1 = nullptr;
		for (auto* component : m_entityManager->GetComponentsTab()[entity1->tab_index]->vec_components)
		{
			if (component->ID == Transform_ID)
			{
				transform1 = static_cast<TransformComponent*>(component);
			}
			if (component->ID == Collider_ID)
			{
				collider1 = static_cast<ColliderComponent*>(component);
			}
			if (component->ID == Health_ID)
			{
				health1 = static_cast<HealthComponent*>(component);
			}
			if (component->ID == Attack_ID)
			{
				attack1 = static_cast<AttackComponent*>(component);
			}
		}
		if (!transform1 || !collider1)
			continue; // Msg d'erreur ?


		for (uint32_t entity2Index = entity1->tab_index + 1; entity2Index < 64000; entity2Index++)
		{
			auto& entity2 = m_entityManager->GetEntityTab()[entity2Index];
			if (!entity2)
				continue;// Msg d'erreur ?

			// Transform & Collider Components
			TransformComponent* transform2 = nullptr;
			ColliderComponent* collider2 = nullptr;
			HealthComponent* health2 = nullptr;
			AttackComponent* attack2 = nullptr;
			for (auto* component : m_entityManager->GetComponentsTab()[entity2Index]->vec_components)
			{
				if (component->ID == Transform_ID)
				{
					transform2 = static_cast<TransformComponent*>(component);
				}
				if (component->ID == Collider_ID)
				{
					collider2 = static_cast<ColliderComponent*>(component);
				}
				if (component->ID == Health_ID)
				{
					health2 = static_cast<HealthComponent*>(component);
				}
				if (component->ID == Attack_ID)
				{
					attack2 = static_cast<AttackComponent*>(component);
				}
			}
			if (!transform2 || !collider2)
				continue;	// Msg d'erreur ?		

			// Test de collision
			if (AABBIntersect(*transform1, *transform2))
			{
				// Ici, vous pouvez ajouter le traitement neessaire en cas de collision
				collider1->m_isColliding = true;
				collider2->m_isColliding = true;

				if (health1 && attack2)
				{
					health1->currentHealth -= attack2->damage;
				}
				if (health2 && attack1)
				{
					health2->currentHealth -= attack1->damage;
				}

				if (collider1->m_isDestructable && entity1)
				{
					//collider1->m_isDestroyed = true;
					m_entityManager->DestroyEntity(entity1);
				}

				if (collider2->m_isDestructable && entity2)
				{
					//collider2->m_isDestroyed = true;
					m_entityManager->DestroyEntity(entity2);
				}

				wchar_t buffer[256];
				swprintf_s(buffer, 256, L"Collision ! posX: %f, posY: %f\n", transform1->m_transform.GetPositionX(), transform1->m_transform.GetPositionY());
				OutputDebugString(buffer);
				//OutputDebugString(L"Collision !\n");
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
	float aMinX = a.m_transform.GetPositionX() - a.m_transform.GetScale().x / 2;
	float aMaxX = a.m_transform.GetPositionX() + a.m_transform.GetScale().x / 2;
	float aMinY = a.m_transform.GetPositionY() - a.m_transform.GetScale().y / 2;
	float aMaxY = a.m_transform.GetPositionY() + a.m_transform.GetScale().y / 2;
	float aMinZ = a.m_transform.GetPositionZ() - a.m_transform.GetScale().z / 2;
	float aMaxZ = a.m_transform.GetPositionZ() + a.m_transform.GetScale().z / 2;

	// Calcul des bornes pour l'objet B
	float bMinX = b.m_transform.GetPositionX() - b.m_transform.GetScale().x / 2;
	float bMaxX = b.m_transform.GetPositionX() + b.m_transform.GetScale().x / 2;
	float bMinY = b.m_transform.GetPositionY() - b.m_transform.GetScale().y / 2;
	float bMaxY = b.m_transform.GetPositionY() + b.m_transform.GetScale().y / 2;
	float bMinZ = b.m_transform.GetPositionZ() - b.m_transform.GetScale().z / 2;
	float bMaxZ = b.m_transform.GetPositionZ() + b.m_transform.GetScale().z / 2;

	// Vérification du chevauchement sur chaque axe
	if (aMaxX < bMinX || aMinX > bMaxX)
		return false;
	if (aMaxY < bMinY || aMinY > bMaxY)
		return false;
	if (aMaxZ < bMinZ || aMinZ > bMaxZ)
		return false;

	return true;
}
