#pragma once

#include "EntityManager.h"

// Syst�me de gestion de la sant�
class HealthSystem
{
public:
    // Met � jour le syst�me en utilisant le temps �coul� (deltaTime)
    void Update(EntityManager* entityManager, float deltaTime);
    //void TakeDamage(HealthComponent* healthComponent, float damage);
};