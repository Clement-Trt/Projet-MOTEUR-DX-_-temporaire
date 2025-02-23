#pragma once

#include "EntityManager.h"

// Système de gestion des attaques
class AttackSystem
{
public:
    // Met à jour le système en utilisant le temps écoulé (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
};