#pragma once

#include "EntityManager.h"

class EnnemyManager;

// Syst�me de gestion de la sant�E
class HealthSystem
{
public:
    // Met �Ejour le syst�me en utilisant le temps �coul�E(deltaTime)
    void Update(EntityManager* entityManager,EnnemyManager* ennemyManager, float deltaTime);
    //void TakeDamage(HealthComponent* healthComponent, float damage);
};