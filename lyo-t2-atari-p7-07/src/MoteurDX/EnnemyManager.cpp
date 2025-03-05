#include "pch.h"
#include "EnnemyManager.h"

EnnemyManager::EnnemyManager()
{
}

void EnnemyManager::Initialize(InitDirect3DApp* app)
{
	m_gameManager = app;
    m_nbEnnemy = 0;
    m_nbEnnemyToSpawn = 0;
    m_waveNb = 1;
    m_startNextWave = true;
    m_gameStart = false;
    //SpawnEnnemy(20,20,20);
	m_entityManager = app->GetEntityManager();
}

void EnnemyManager::Update()
{
    if (!m_gameStart) 
    {
        // VK_key pour les touches comme enter ou shift
        if (InputManager::GetKeyIsPressed(VK_RETURN)) { m_gameStart = true; };

        // Skip wave at start
        if (InputManager::GetKeyIsPressed('1')) { m_waveNb = 1; m_gameStart = true; };
        if (InputManager::GetKeyIsPressed('2')) { m_waveNb = 2; m_gameStart = true; };
        if (InputManager::GetKeyIsPressed('3')) { m_waveNb = 3; m_gameStart = true; };
    }
    else
    {
        WaveSystem();
        EnnemyAttackSystem();
    }
}

void EnnemyManager::EnnemyAttackSystem()
{
    // Premiere boucle : rechercher la transform du joueur
    m_playerTransform = nullptr;
    for (auto& entity : m_entityManager->GetEntityTab())
    {
        if (entity == nullptr)
            continue;

        auto components = m_entityManager->GetComponentsTab()[entity->tab_index]->vec_components;
        for (auto& comp : components)
        {
            if (comp->ID == Player_ID)
            {
                m_player = entity;
                // Trouver la transform dans les composants de cette entite
                for (auto& pcomp : components)
                {
                    if (pcomp->ID == Transform_ID)
                    {
                        m_playerTransform = static_cast<TransformComponent*>(pcomp.get());
                        //OutputDebugString(L"PlayerTransformFound !\n");
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
        for (auto& comp : components)
        {
            if (comp->ID == Ennemy_ID)
                isEnemy = true; 
            if (comp->ID == Transform_ID)
                enemyTransform = static_cast<TransformComponent*>(comp.get());
        }

        if (isEnemy && enemyTransform != nullptr)
        {
            // Faire suivre l'ennemi vers le joueur
            enemyTransform->m_transform.FollowTarget(m_playerTransform->m_transform.GetPositionF3(), 0.1f);
            enemyTransform->m_transform.LookAt(m_playerTransform->m_transform.GetPositionF3());

            AttackComponent* attack = nullptr;
            auto& playerComponents = m_gameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components;
            for (auto& component : playerComponents)
            {
                if (component->ID == Attack_ID)
                {
                    attack = static_cast<AttackComponent*>(component.get());
                    attack->projectileTexture = L"RedBeamTexture";
                    break;
                }
            }
            if (attack)
            {
                // Declencher l'attaque en definissant le flag et en indiquant la cible
                attack->attackRequested = true;
                attack->attackCooldown = 0.9f;
                attack->damage = 2;

                attack->projectileSpeed = 1; // 1
                attack->projectileSizeX = 0.2f; // 0.2f
                attack->projectileSizeY = 0.2f; // 0.2f
                attack->projectileSizeZ = 1.0f; // 1.0f
            }
        }
    }
}

void EnnemyManager::SpawnEnnemy(float posX, float posY, float posZ)
{
    Entity* ennemy = m_entityManager->CreateEntity();
    m_entityManager->AddComponent<TransformComponent>(ennemy);
    m_entityManager->AddComponent<MeshComponent>(ennemy);
    m_entityManager->AddComponent<ColliderComponent>(ennemy);
    m_entityManager->AddComponent<EnnemyComponent>(ennemy);
    m_entityManager->AddComponent<AttackComponent>(ennemy);
    m_entityManager->AddComponent<HealthComponent>(ennemy);

    for (auto& comp : m_gameManager->GetEntityManager()->GetComponentToAddTab()[ennemy->tab_index]->vec_components)
    {
        if (comp->ID == Mesh_ID)
        {
            MeshComponent* mesh = static_cast<MeshComponent*>(comp.get());
            mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
            mesh->textureID = L"DroneTexture";
        }
        if (comp->ID == Transform_ID)
        {
            TransformComponent* transform = static_cast<TransformComponent*>(comp.get());
            transform->m_transform.Scale(2.f, 2.f, 2.f);
            transform->m_transform.Move(posZ, posX, posY);
        }
        if (comp->ID == Health_ID)
        {
            HealthComponent* healthComp = static_cast<HealthComponent*>(comp.get());
            healthComp->maxHealth = healthComp->currentHealth = 10;
        }
    }
}

void EnnemyManager::SpawnEnnemyBoss(float posX, float posY, float posZ)
{
    Entity* ennemy = m_entityManager->CreateEntity();
    m_entityManager->AddComponent<TransformComponent>(ennemy);
    m_entityManager->AddComponent<MeshComponent>(ennemy);
    m_entityManager->AddComponent<ColliderComponent>(ennemy);
    m_entityManager->AddComponent<EnnemyComponent>(ennemy);
    m_entityManager->AddComponent<AttackComponent>(ennemy);
    m_entityManager->AddComponent<HealthComponent>(ennemy);

    for (auto& comp : m_gameManager->GetEntityManager()->GetComponentToAddTab()[ennemy->tab_index]->vec_components)
    {
        if (comp->ID == Mesh_ID)
        {
            MeshComponent* mesh = static_cast<MeshComponent*>(comp.get());
            mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
            mesh->textureID = L"DroneTexture";
        }
        if (comp->ID == Transform_ID)
        {
            TransformComponent* transform = static_cast<TransformComponent*>(comp.get());
            transform->m_transform.Scale(10.f, 10.f, 10.f);
            transform->m_transform.Move(posZ, posX, posY);
        }
        if (comp->ID == Health_ID)
        {
            HealthComponent* healthComp = static_cast<HealthComponent*>(comp.get());
            healthComp->maxHealth = 100;
            healthComp->currentHealth = 100;
        }
    }
}

void EnnemyManager::WaveSystem()
{
    if (m_startNextWave) 
    {
        switch (m_waveNb)
        {
        case 1:
            m_nbEnnemyToSpawn = 5;

            break;
        case 2:
            m_nbEnnemyToSpawn = 10;

            break;
        case 3:
            m_nbEnnemyToSpawn = 20;

            break;
        default:

            break;
        }

        while (m_nbEnnemy < m_nbEnnemyToSpawn)
        {
            float randPosX = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            float randPosY = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            float randPosZ = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            SpawnEnnemy(randPosX, randPosY, randPosZ);
            m_nbEnnemy++;
            OutputDebugString(L"SpawnEnnemy\n");
        }
        if (m_waveNb == 3)
        {
            float randPosX = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            float randPosY = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            float randPosZ = -50 + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (50 - -50);
            SpawnEnnemyBoss(randPosX, randPosY, randPosZ);
        }

        m_startNextWave = false;
    }
    else
    {
        if (m_nbEnnemy <= 0 && m_waveNb < 3)
        {
            m_waveNb++;				
            wchar_t buffer[256];
            swprintf_s(buffer, 256, L"NB waves %d\r\n", m_waveNb);
            OutputDebugString(buffer);
            m_startNextWave = true;
        }
    }
}
