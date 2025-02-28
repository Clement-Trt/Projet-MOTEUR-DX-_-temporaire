#pragma once

//#include "EntityManager.h"
class InitDirect3DApp;
class EntityManager;
// Systeme de gestion des attaques
class AttackSystem
{
protected:

    InitDirect3DApp* mGM;

public:
    void Initialize(InitDirect3DApp* gameManager);

    // Met a jour le systeme en utilisant le temps ecoule (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
};