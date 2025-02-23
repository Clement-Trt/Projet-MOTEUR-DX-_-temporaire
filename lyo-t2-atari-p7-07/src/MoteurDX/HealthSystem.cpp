#include "pch.h"
#include "HealthSystem.h"
#include "Components.h" // Pour HealthComponent

void HealthSystem::Update(EntityManager* entityManager, float deltaTime)
{
    // Parcourir toutes les entités
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        // Vérifier si l'entité possède le composant Health
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
                // Exemple de logique : détruire l'entité si la santé est épuisée
                if (health->currentHealth <= 0)
                {
                    entityManager->ToDestroy(entity);
                }

                // Vous pouvez par exemple ajouter une régénération de la santé :
                // float regenRate = 5.0f; // points de vie par seconde
                // health->currentHealth = std::min(health->currentHealth + regenRate * deltaTime, float(health->maxHealth));
            }
        }
    }
}