#include "pch.h"

#include "EntityManager.h"
#include <iostream>

Entity* EntityManager::CreateEntity() {

	Entity* entity = new Entity;

	entity->tab_index = entitiesToAddIndex * -1;
	entity->id = entitiesToAddIndex - 1;
	tab_entitiesToAdd.push_back(entity);

	EntityComponents* newComponents = new EntityComponents;
	newComponents->mask = COMPONENT_NONE;
	newComponents->tab_index = entitiesToAddIndex;

	tab_compToAdd.push_back(newComponents);

	//entity->tab_index = entityNb;
	//entity->id = entityNb + 1;

	//tab_entity[entity->tab_index] = entity; // Ajout de l'entity dans le tableau d'entities

	//newComponents->mask = COMPONENT_NONE;
	//newComponents->tab_index = entityNb;
	//tab_Components[entityNb] = newComponents; // Ajout des components (vides) de l'entity dans la case correspondant du tableau de composants

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

//void EntityManager::AddComponentsToTab(EntityComponents* components)
//{
//	components->mask = COMPONENT_NONE;
//	components->tab_index = entityNb;
//	tab_Components[entityNb] = components; // Ajout des components (vides) de l'entity dans la case correspondant du tableau de composants
//
//	//entityNb++;
//	//return entity;
//}

int EntityManager::DestroyEntity(Entity* entity) {
	int index = entity->tab_index;
	int lastIndex = entityNb - 1;

	if (index > lastIndex) {
		std::cerr << "Index invalide." << std::endl;
		return 10000;
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

	return entityNb;
}

void EntityManager::ToDestroy(Entity* entity)
{
	tab_toDestroy.push_back(entity);
}

void EntityManager::AddComponent(Entity* entity, ComponentMask componentMask) {

	if (entity->id < 0)
	{
		tab_compToAdd[entity->tab_index]->mask |= componentMask;

		if (componentMask & COMPONENT_TRANSFORM)
		{
			if (tab_compToAdd[entity->tab_index]->tab_components[Transform_index] == nullptr)
			{
				TransformComponent* newTransformComp = new TransformComponent;
				tab_compToAdd[entity->tab_index]->tab_components[Transform_index] = newTransformComp;
			}
		}

		if (componentMask & COMPONENT_MESH)
		{
			if (tab_compToAdd[entity->tab_index]->tab_components[Mesh_index] == nullptr)
			{
				MeshComponent* newMeshComp = new MeshComponent;
				tab_compToAdd[entity->tab_index]->tab_components[Mesh_index] = newMeshComp;
			}
		}

		if (componentMask & COMPONENT_VELOCITY)
		{
			if (tab_compToAdd[entity->tab_index]->tab_components[Velocity_index] == nullptr)
			{
				VelocityComponent* newVelocityComp = new VelocityComponent;
				tab_compToAdd[entity->tab_index]->tab_components[Velocity_index] = newVelocityComp;
			}
		}
	}
	else // id > 0
	{
		tab_Components[entity->tab_index]->mask |= componentMask;


		if (componentMask & COMPONENT_TRANSFORM)
		{
			if (tab_Components[entity->tab_index]->tab_components[Transform_index] == nullptr)
			{
				TransformComponent* newTransformComp = new TransformComponent;
				tab_Components[entity->tab_index]->tab_components[Transform_index] = newTransformComp;
			}
		}

		if (componentMask & COMPONENT_MESH)
		{
			if (tab_Components[entity->tab_index]->tab_components[Mesh_index] == nullptr)
			{
				MeshComponent* newMeshComp = new MeshComponent;
				tab_Components[entity->tab_index]->tab_components[Mesh_index] = newMeshComp;
			}
		}

		if (componentMask & COMPONENT_VELOCITY)
		{
			if (tab_Components[entity->tab_index]->tab_components[Velocity_index] == nullptr)
			{
				VelocityComponent* newVelocityComp = new VelocityComponent;
				tab_Components[entity->tab_index]->tab_components[Velocity_index] = newVelocityComp;
			}
		}
	}
}

void EntityManager::RemoveComponent(Entity* entity, ComponentMask componentMask) {
	if (entity->id < 0)
	{
		tab_compToAdd[entity->tab_index]->mask &= ~componentMask;

		if (componentMask & COMPONENT_TRANSFORM)
		{
			auto& component = tab_compToAdd[entity->tab_index]->tab_components[Transform_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}

		if (componentMask & COMPONENT_MESH)
		{
			auto& component = tab_compToAdd[entity->tab_index]->tab_components[Mesh_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}

		if (componentMask & COMPONENT_VELOCITY)
		{
			auto& component = tab_compToAdd[entity->tab_index]->tab_components[Velocity_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}
	}
	else {
		tab_Components[entity->tab_index]->mask &= ~componentMask;

		if (componentMask & COMPONENT_TRANSFORM)
		{
			auto& component = tab_Components[entity->tab_index]->tab_components[Transform_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}

		if (componentMask & COMPONENT_MESH)
		{
			auto& component = tab_Components[entity->tab_index]->tab_components[Mesh_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}

		if (componentMask & COMPONENT_VELOCITY)
		{
			auto& component = tab_Components[entity->tab_index]->tab_components[Velocity_index];

			if (component != nullptr)
			{
				delete component;
				component = nullptr;
			}
		}
	}
	/*int compIndex = 0;
	switch (componentMask)
	{
	case COMPONENT_TRANSFORM:
		compIndex = Transform_index;
		break;

	case COMPONENT_MESH:
		compIndex = COMPONENT_MESH;
		break;

	case COMPONENT_VELOCITY:
		compIndex = COMPONENT_VELOCITY;
		break;

	case COMPONENT_HEALTH:
		compIndex = COMPONENT_HEALTH;
		break;

	case COMPONENT_HEAL:
		compIndex = COMPONENT_HEAL;
		break;

	default:
		break;
	}

	auto& component = tab_Components[entity->tab_index]->tab_components[compIndex];

	if (component != nullptr)
	{
		delete component;
		component = nullptr;
	}*/
}

bool EntityManager::HasComponent(Entity* entity, ComponentMask componentMask) const {

	return (tab_Components[entity->tab_index]->mask & componentMask) == componentMask;
}


