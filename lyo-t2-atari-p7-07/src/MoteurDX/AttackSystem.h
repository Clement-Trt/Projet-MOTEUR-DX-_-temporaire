#pragma once

#include "EntityManager.h"

// Syst�me de gestion des attaques
class AttackSystem
{
public:
    // Met � jour le syst�me en utilisant le temps �coul� (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
};