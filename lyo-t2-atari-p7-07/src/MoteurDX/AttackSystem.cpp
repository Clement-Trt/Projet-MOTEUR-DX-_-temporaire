#include "pch.h"
#include "AttackSystem.h"
#include "InitDirect3DApp.h"
#include "Components.h" // Pour AttackComponent

void AttackSystem::Initialize(InitDirect3DApp* gameManager)
{
    mGM = gameManager;
}

void AttackSystem::Update(EntityManager* entityManager, float deltaTime)
{
    for (Entity* entity : entityManager->GetEntityTab())
    {
        if (!entity)
            continue;

        if (entityManager->HasComponent(entity, COMPONENT_ATTACK))
        {
            AttackComponent* attack = nullptr;
            TransformComponent* entityTransform = nullptr;
            auto& compTab = entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto* comp : compTab)
            {
                if (comp->ID == Attack_ID)
                {
                    attack = static_cast<AttackComponent*>(comp);
                }
                if (comp->ID == Transform_ID)
                {
                    entityTransform = static_cast<TransformComponent*>(comp);
                }
            }
            if (attack)
            {
                // Toujours incrémenter le temps écoulé
                attack->timeSinceLastAttack += deltaTime;  // a mettre en dehors de la boucle, non ?? /!\/!\/!\/!\/!\/!\/

                // Si une attaque est demandée et que le cooldown est respecté
                if (attack->attackRequested && attack->timeSinceLastAttack >= attack->attackCooldown)
                {
                    /*if (attack->targetEntity != nullptr)
                    {*/
                        //MessageBox(0, L"Attaque du joueur !", 0, 0);

                        Entity* newBullet = entityManager->CreateEntity();
                        
                        entityManager->AddComponent<TransformComponent>(newBullet);
                        entityManager->AddComponent<MeshComponent>(newBullet);
                        entityManager->AddComponent<VelocityComponent>(newBullet);
                        entityManager->AddComponent<ColliderComponent>(newBullet);
                        entityManager->AddComponent<AttackComponent>(newBullet);


                        for (auto& component : entityManager->GetComponentToAddTab()[newBullet->tab_index]->vec_components)
                        {
                            if (component->ID == Mesh_ID)
                            {
                                MeshComponent* mesh = static_cast<MeshComponent*>(component);
                                mesh->m_cubeMesh = mGM->GetFactory()->CreateCube();
                                mesh->textureID = L"PlayerTexture"; // On assigne la texture
                            }
                            if (component->ID == Transform_ID)
                            {
                                TransformComponent* transform = static_cast<TransformComponent*>(component);
                                transform->m_transform = entityTransform->m_transform;
                                transform->m_transform.Move(1, 0, 0);
                                transform->m_transform.Scale(0.5f, 0.5f, 0.5f);
                            }
                            if (component->ID == Velocity_ID)
                            {
                                VelocityComponent* vel = static_cast<VelocityComponent*>(component);
                                vel->vx = 0.0f;
                                vel->vy = 0.0f;
                                vel->vz = 2.0f;
                            }
                            if (component->ID == Collider_ID)
                            {
                                ColliderComponent* collider = static_cast<ColliderComponent*>(component);
                                collider->m_isDestructable = true;
                            }
                            if (component->ID == Attack_ID)
                            {
                                AttackComponent* attack = static_cast<AttackComponent*>(component);
                            }
                        }


                       /* HealthComponent* health = nullptr;
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
                        }*/
                    //}
                    // Réinitialiser le cooldown et le flag d'attaque
                    attack->timeSinceLastAttack = 0.0f;
                    attack->attackRequested = false;
                    attack->targetEntity = nullptr;
                }
            }
        }
    }
}
