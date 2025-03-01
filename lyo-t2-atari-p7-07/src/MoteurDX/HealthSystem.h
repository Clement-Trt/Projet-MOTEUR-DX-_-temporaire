#pragma once

#include "EntityManager.h"

// Système de gestion de la santE
class HealthSystem
{
public:
    // Met Ejour le système en utilisant le temps écoulE(deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
    //void TakeDamage(HealthComponent* healthComponent, float damage);
};