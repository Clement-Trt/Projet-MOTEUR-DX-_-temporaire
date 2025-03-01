#include "pch.h"
#include "HealthSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h" // Pour HealthComponent

void HealthSystem::Initialize(InitDirect3DApp* gameManager)
{
    m_gameManager = gameManager;
}

void HealthSystem::Update(float deltaTime)
{
    EntityManager* entityManager = m_gameManager->GetEntityManager();

    // Parcourir toutes les entit�s
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        // V�rifier si l'entit� poss�de le composant Health
        if (entityManager->HasComponent(entity, COMPONENT_HEALTH))
        {
            HealthComponent* health = nullptr;
            auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto* comp : compTab)
            {
                if (comp->ID == Health_ID)
                {
                    health = static_cast<HealthComponent*>(comp);
                    break;
                }
            }
            if (health)
            {
                // Exemple de logique : d�truire l'entit� si la sant� est �puis�e
                if (health->currentHealth <= 0)
                {
                    entityManager->ToDestroy(entity);
                }

                // Vous pouvez par exemple ajouter une r�g�n�ration de la sant� :
                // float regenRate = 5.0f; // points de vie par seconde
                // health->currentHealth = std::min(health->currentHealth + regenRate * deltaTime, float(health->maxHealth));
            }
        }
    }
}

