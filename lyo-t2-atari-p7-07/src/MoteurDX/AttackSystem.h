#pragma once

//#include "EntityManager.h"
class InitDirect3DApp;
class EntityManager;
// Syst�me de gestion des attaques
class AttackSystem
{
protected:

    InitDirect3DApp* mGM;

public:
    void Initialize(InitDirect3DApp* gameManager);

    // Met � jour le syst�me en utilisant le temps �coul� (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
};