#include "pch.h"
#include "EnnemyManager.h"

EnnemyManager::EnnemyManager()
{
}

void EnnemyManager::InitEnnemyManager(EntityManager* entityManager, InitDirect3DApp* app)
{
	m_entityManager = entityManager;
	m_gameManager = app;
}

void EnnemyManager::Update()
{
    // Premiere boucle : rechercher la transform du joueur
    m_playerTransform = nullptr;
    for (auto& entity : m_entityManager->GetEntityTab())
    {
        if (entity == nullptr)
            continue;

        auto components = m_entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
        for (auto* comp : components)
        {
            if (comp->ID == Player_ID)
            {
                m_player = entity;
                // Trouver la transform dans les composants de cette entite
                for (auto* pcomp : components)
                {
                    if (pcomp->ID == Transform_ID)
                    {
                        m_playerTransform = static_cast<TransformComponent*>(pcomp);
                        OutputDebugString(L"PlayerTransformFound !\n");
                        break;
                    }
                }
            }
            if (m_playerTransform != nullptr)
                break;
        }
        if (m_playerTransform != nullptr)
            break;
    }

    // Si aucun joueur n'a ete trouve, on sort
    if (m_playerTransform == nullptr)
    {
        OutputDebugString(L"No PlayerTransform found, skipping enemy update\n");
        return;
    }

    // Deuxieme boucle : mettre a jour les ennemis
    for (auto& entity : m_entityManager->GetEntityTab())
    {
        if (entity == nullptr)
            continue;

        auto components = m_entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
        bool isEnemy = false;
        TransformComponent* enemyTransform = nullptr;
        for (auto* comp : components)
        {
            if (comp->ID == Ennemy_ID)
                isEnemy = true;
            if (comp->ID == Transform_ID)
                enemyTransform = static_cast<TransformComponent*>(comp);
        }

        if (isEnemy && enemyTransform != nullptr)
        {
            // Faire suivre l'ennemi vers le joueur
            enemyTransform->m_transform.FollowTarget(m_playerTransform->m_transform.GetPositionF3(), 0.1f);
            enemyTransform->m_transform.LookAt(m_playerTransform->m_transform.GetPositionF3());

            AttackComponent* attack = nullptr;
            auto& playerComponents = m_gameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto* component : playerComponents)
            {
                if (component->ID == Attack_ID)
                {
                    attack = static_cast<AttackComponent*>(component);
                    break;
                }
            }
            if (attack)
            {
                // Declencher l'attaque en definissant le flag et en indiquant la cible
                attack->attackRequested = true;
                attack->targetEntity = m_player;
            }
        }
    }
}
