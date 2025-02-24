#pragma once

//#include "EntityManager.h"
class InitDirect3DApp;
class EntityManager;
// Système de gestion des attaques
class AttackSystem
{
protected:

    InitDirect3DApp* mGM;

public:
    void Initialize(InitDirect3DApp* gameManager);

    // Met à jour le système en utilisant le temps écoulé (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
};