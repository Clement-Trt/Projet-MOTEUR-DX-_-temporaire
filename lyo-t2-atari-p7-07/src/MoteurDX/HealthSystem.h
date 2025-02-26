#pragma once

#include "EntityManager.h"

// Système de gestion de la santé
class HealthSystem
{
public:
    // Met à jour le système en utilisant le temps écoulé (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
    //void TakeDamage(HealthComponent* healthComponent, float damage);
};