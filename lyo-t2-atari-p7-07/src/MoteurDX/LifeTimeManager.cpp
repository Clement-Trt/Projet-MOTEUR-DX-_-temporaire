#include "pch.h"
#include "LifeTimeManager.h"
#include "InitDirect3DApp.h"
#include "Components.h"

void LifeTimeManager::Initialize(InitDirect3DApp* gameManager)
{
	mGM = gameManager;
}

void LifeTimeManager::Update(float deltaTime)
{
	EntityManager* entityManager = mGM->GetEntityManager();
	
	for (Entity* entity : entityManager->GetEntityTab())
	{
		if (!entity)
			continue;

		if (entityManager->HasComponent(entity, COMPONENT_LIFETIME))
		{
			LifeTimeComponent* lifeTime = nullptr;
			//auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
			for (auto* comp : entityManager->GetComponentsTab()[entity->tab_index]->vec_components)
			{
				if (comp->ID == LifeTime_ID)
				{
					lifeTime = static_cast<LifeTimeComponent*>(comp);

					if (lifeTime->lifeTime > 0) 
					{
						lifeTime->lifeTime -= 1 * deltaTime;
					}
					else
					{
						entityManager->ToDestroy(entity);
					}
				}
			}
		}
	}
}
