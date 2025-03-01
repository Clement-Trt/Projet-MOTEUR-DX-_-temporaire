#include "pch.h"
#include "HealthSystem.h"
#include "Components.h" // Pour HealthComponent
#include "EnnemyManager.h"

void HealthSystem::Update(EntityManager* entityManager, EnnemyManager* ennemyManager, float deltaTime)
{
    // Parcourir toutes les entités
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        // Vérifier si l'entitEpossède le composant Health
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
                    //break;
                }
                if (comp->ID == Ennemy_ID)
                {
                    ennemy = static_cast<EnnemyComponent*>(comp);
                    //break;
                }
            }
            if (health)
            {
                // Exemple de logique : détruire l'entitEsi la santEest épuisée
                if (health->currentHealth <= 0)
                {
                    // baisse le compte des ennemies
                    if (ennemy != nullptr) 
                    {
                        ennemyManager->SetNbEnnemy(ennemyManager->GetNbEnnemy() - 1);
                    }
                    entityManager->ToDestroy(entity);
                }

                // Vous pouvez par exemple ajouter une régénération de la santE:
                // float regenRate = 5.0f; // points de vie par seconde
                // health->currentHealth = std::min(health->currentHealth + regenRate * deltaTime, float(health->maxHealth));
            }
        }
    }
}

//void HealthSystem::TakeDamage(HealthComponent* healthComponent, float damage)
//{
//    healthComponent->currentHealth -= damage;
//}