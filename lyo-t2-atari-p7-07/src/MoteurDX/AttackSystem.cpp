#include "pch.h"
#include "AttackSystem.h"
#include "Components.h" // Pour AttackComponent

void AttackSystem::Update(EntityManager* entityManager, float deltaTime)
{
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        if (entityManager->HasComponent(entity, COMPONENT_ATTACK))
        {
            AttackComponent* attack = nullptr;
            auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto* comp : compTab)
            {
                if (comp->ID == Attack_ID)
                {
                    attack = static_cast<AttackComponent*>(comp);
                    break;
                }
            }
            if (attack)
            {
                // Toujours incrémenter le temps écoulé
                attack->timeSinceLastAttack += deltaTime;

                // Si une attaque est demandée et que le cooldown est respecté
                if (attack->attackRequested && attack->timeSinceLastAttack >= attack->attackCooldown)
                {
                    if (attack->targetEntity != nullptr)
                    {
                        MessageBox(0, L"Attaque du joueur !", 0, 0);

                        HealthComponent* health = nullptr;
                        auto& targetCompTab = entityManager->GetComponentsTab()[attack->targetEntity->tab_index]->vec_components;
                        for (auto* comp : targetCompTab)
                        {
                            if (comp->ID == Health_ID)
                            {
                                health = static_cast<HealthComponent*>(comp);
                                break;
                            }
                        }
                        if (health)
                        {
                            health->currentHealth -= attack->damage;
                            std::cout << "Entity " << entity->id << " attaque la cible "
                                << attack->targetEntity->id << " ! Nouvelle santé: "
                                << health->currentHealth << std::endl;
                        }
                    }
                    // Réinitialiser le cooldown et le flag d'attaque
                    attack->timeSinceLastAttack = 0.0f;
                    attack->attackRequested = false;
                    attack->targetEntity = nullptr;
                }
            }
        }
    }
}
