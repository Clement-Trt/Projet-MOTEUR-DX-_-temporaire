#pragma once

#include <vector>
#include "Components.h"

using ComponentMask = uint32_t;

//enum ComponentType {
//	COMPONENT_NONE = 0,
//	COMPONENT_TRANSFORM = 1 << 0,
//	COMPONENT_MESH = 1 << 1,
//	COMPONENT_VELOCITY = 1 << 2,
//	COMPONENT_HEALTH = 1 << 3,
//
//	COMPONENT_HEAL = 1 << 4,		// Exemple
//
//	TOTALCOMPONENT
//};
//
//enum ComponentIndex
//{
//	Transform_index,
//	Mesh_index,
//	Velocity_index,
//	Health_index,
//
//	Heal_index		// Exemple
//};

class Entity
{
public:
	int32_t id = 0;
	uint32_t tab_index = 0;
};

struct EntityComponents
{
	uint32_t tab_index = 0;
	ComponentMask mask = 0;
	Component* tab_components[TOTALCOMPONENT] = { nullptr };
};


class EntityManager
{
private:

	uint32_t entityNb = 0; // Positif, ce sont les entity qui existent actuellement dans le jeu
	int32_t entitiesToAddIndex = 0; // Negatif pour savoir qu'elles ne sont pas encore ajoutées 

	Entity* tab_entity[64000] = { nullptr };

	EntityComponents* tab_Components[64000] = { nullptr };

	std::vector<Entity*> tab_toDestroy;
	std::vector<Entity*> tab_entitiesToAdd;
	std::vector<EntityComponents*> tab_compToAdd;

public:

	Entity*(&GetEntityTab())[64000] { return tab_entity; }

	EntityComponents*(&GetComponentsTab())[64000] {	return tab_Components; }

	std::vector<Entity*>& GetToDestroyTab() { return tab_toDestroy; }
	std::vector<Entity*>& GetEntityToAddTab() { return tab_entitiesToAdd; }
	std::vector<EntityComponents*>& GetComponentToAddTab() { return tab_compToAdd; }

	uint32_t GetNbEntity() { return entityNb; }

	// Cree une entite et retourne son ID
	Entity* CreateEntity();

	// Detruit une entite (supprime sa signature de composants)
	void ToDestroy(Entity* entity);
	int DestroyEntity(Entity* entity);

	// Ajoute un composant a une entite (en "ou"-ant le bit)
	void AddComponent(Entity* entity, ComponentMask componentMask);

	// Retire un composant a une entite (en effaçant le bit)
	void RemoveComponent(Entity* entity, ComponentMask componentMask);

	// Verifie si l'entite possede le(s) composant(s) indique(s)
	bool HasComponent(Entity* entity, ComponentMask componentMask) const;

	void ResetEntitiesToAdd() { entitiesToAddIndex = 0; }

	void AddEntityToTab(Entity* entity, EntityComponents* components);
};