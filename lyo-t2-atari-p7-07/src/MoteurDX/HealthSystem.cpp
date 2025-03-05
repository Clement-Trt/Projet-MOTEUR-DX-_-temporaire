#include "pch.h"
#include "HealthSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h"
#include "EnnemyManager.h"

void HealthSystem::Initialize(InitDirect3DApp* gameManager)
{
    mp_gameManager = gameManager;
}

void HealthSystem::Update(float deltaTime)
{
    EntityManager* entityManager = mp_gameManager->GetEntityManager();
    EnnemyManager* ennemyManager = mp_gameManager->GetEnnemyManager();

    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        if (entityManager->HasComponent(entity, COMPONENT_HEALTH))
        {
            HealthComponent* health = nullptr;
            EnnemyComponent* ennemy = nullptr;
            auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto* comp : compTab)
            {
                if (comp->ID == Health_ID)
                {
                    health = static_cast<HealthComponent*>(comp);
                }
                if (comp->ID == Ennemy_ID)
                {
                    ennemy = static_cast<EnnemyComponent*>(comp);
                }
            }
            if (health)
            {
                if (health->currentHealth <= 0)
                {
                    // baisse le compte des ennemies
                    if (ennemy != nullptr) 
                    {
                        ennemyManager->SetNbEnnemy(ennemyManager->GetNbEnnemy() - 1);
                    }
                    // detruire l'entity
                    entityManager->ToDestroy(entity);
                }
            }
        }
    }
}

