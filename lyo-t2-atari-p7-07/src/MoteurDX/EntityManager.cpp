#include "pch.h"

#include "EntityManager.h"
#include <iostream>

EntityManager::~EntityManager()
{	 
	for (auto& entity : tab_entity)
	{
		delete entity;
		entity = nullptr;
	}
	
	for (auto& entity : tab_Components)
	{
		delete entity;
		entity = nullptr;
	}
	
	for (auto& entity : tab_toDestroy)
	{
		delete entity;
		entity = nullptr;
	}
	tab_toDestroy.clear();

	for (auto& entity : tab_entitiesToAdd)
	{
		delete entity;
		entity = nullptr;
	}
	tab_entitiesToAdd.clear();
	for (auto& component :tab_compToAdd)
	{
		delete component;
		component = nullptr;
	}
	tab_compToAdd.clear();
}

Entity* EntityManager::CreateEntity() {

	Entity* entity = new Entity;

	entity->tab_index = entitiesToAddIndex * -1;
	entity->id = entitiesToAddIndex - 1;
	tab_entitiesToAdd.push_back(entity);

	EntityComponents* newComponents = new EntityComponents;
	newComponents->mask = COMPONENT_NONE;
	newComponents->tab_index = entitiesToAddIndex;

	tab_compToAdd.push_back(newComponents);

	entitiesToAddIndex--;
	return entity;
}

void EntityManager::AddEntityToTab(Entity* entity, EntityComponents* components)
{
	entity->tab_index = entityNb;
	entity->id = entityNb + 1;
	tab_entity[entity->tab_index] = entity; // Ajout de l'entity dans le tableau d'entities

	components->tab_index = entityNb;
	tab_Components[entityNb] = components; // Ajout des components (vides) de l'entity dans la case correspondant du tableau de composants

	entityNb++;
}

void EntityManager::DestroyEntity(Entity* entity) {
	int index = entity->tab_index;
	int lastIndex = entityNb - 1;

	if (index > lastIndex) {
		std::cerr << "Index of entity to destroy invalid." << std::endl;
		return;
	}

	tab_entity[lastIndex]->tab_index = index;
	*tab_entity[index] = *tab_entity[lastIndex];

	tab_Components[lastIndex]->tab_index = index;
	*tab_Components[index] = *tab_Components[lastIndex];

	tab_entity[lastIndex] = nullptr;
	delete tab_entity[lastIndex];

	tab_Components[lastIndex] = nullptr;
	delete tab_Components[lastIndex];
	int newEntityToPointAt = --entityNb;
}

void EntityManager::ToDestroy(Entity* entity)
{
	tab_toDestroy.push_back(entity);
}

bool EntityManager::HasComponent(Entity* entity, ComponentMask componentMask) const {

	return (tab_Components[entity->tab_index]->mask & componentMask) == componentMask;
}


