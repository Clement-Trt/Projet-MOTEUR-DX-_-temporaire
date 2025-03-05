#pragma once

#include "Components.h"

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
	std::vector<std::unique_ptr<Component>> vec_components;
};


class EntityManager
{
private:

	uint32_t entityNb = 0; // Positif, ce sont les entity qui existent actuellement dans le jeu
	int32_t entitiesToAddID = 0; // Negatif pour savoir qu'elles ne sont pas encore ajoutées 

	Entity* tab_entity[64000] = { nullptr };

	std::array<std::unique_ptr<EntityComponents>, 64000> tab_Components;
	//EntityComponents* tab_Components[64000] = { nullptr };


	std::vector<Entity*> tab_toDestroy;
	std::vector<Entity*> tab_entitiesToAdd;
	//std::vector<EntityComponents*> tab_compToAdd;
	std::vector<std::unique_ptr<EntityComponents>> tab_compToAdd;

public:
	EntityManager();
	~EntityManager();

	Entity* (&GetEntityTab())[64000] { return tab_entity; }

	//EntityComponents* (&GetComponentsTab())[64000] { return tab_Components; }
	std::array<std::unique_ptr<EntityComponents>, 64000>& GetComponentsTab() { return tab_Components; }


	std::vector<Entity*>& GetToDestroyTab() { return tab_toDestroy; }
	std::vector<Entity*>& GetEntityToAddTab() { return tab_entitiesToAdd; }
	std::vector<std::unique_ptr<EntityComponents>>& GetComponentToAddTab() { return tab_compToAdd; }

	uint32_t GetNbEntity() { return entityNb; }

	// Cree une entite et retourne son ID
	Entity* CreateEntity();

	// Detruit une entite (supprime sa signature de composants)
	void ToDestroy(Entity* entity);
	void DestroyEntity(Entity* entity/*, EntityComponents* components*/);

	// Verifie si l'entite possede le(s) composant(s) indique(s)
	bool HasComponent(Entity* entity, ComponentMask componentMask) const;

	void ResetEntitiesToAdd() { entitiesToAddID = 0; }

	void AddEntityToTab(Entity* entity, std::unique_ptr<EntityComponents> components);

	template <typename T>
	void AddComponent(Entity* entity)
	{
		//T* newComp = new T;
		auto newComp = std::make_unique<T>();

		if (entity->id < 0) // Entity a ajouter
		{
			for (auto& component : tab_compToAdd[entity->tab_index]->vec_components)
			{
				if (newComp->ID == component->ID)
				{
					std::cout << "Entity already has this component" << std::endl;
					return;
				}
			}
			tab_compToAdd[entity->tab_index]->mask |= newComp->mask;

			tab_compToAdd[entity->tab_index]->vec_components.push_back(std::move(newComp));
		}
		else // entity existante
		{
			assert(tab_Components[entity->tab_index] && "tab_Components[index] est nullptr !");
			for (auto& component : tab_Components[entity->tab_index]->vec_components)
			{
				if (newComp->ID == component->ID)
				{
					return;
				}
			}
			tab_Components[entity->tab_index]->mask |= newComp->mask;

			tab_Components[entity->tab_index]->vec_components.push_back(std::move(newComp));
		}
	}

	template <typename T>
	void RemoveComponent(Entity* entity)
	{
		//T* compToRemove = new T;
		auto compToRemove = std::make_unique<T>();

		if (entity->id < 0) // Entity a ajouter
		{
			auto& compVec = tab_compToAdd[entity->tab_index]->vec_components;
			for (size_t i = 0; i < compVec.size(); ++i)
			{
				if (compToRemove->ID == compVec[i]->ID)
				{
					tab_compToAdd[entity->tab_index]->mask &= ~compToRemove->mask;
					compVec.erase(compVec.begin() + i);
					return;
				}
			}

			
		}
		else // entity existante
		{
			assert(tab_Components[entity->tab_index] && "tab_Components[index] est nullptr !");
			auto& compVec = tab_Components[entity->tab_index]->vec_components;
			for (size_t i = 0; i < compVec.size(); ++i)
			{
				if (compToRemove->ID == compVec[i]->ID)
				{
					tab_Components[entity->tab_index]->mask &= ~compToRemove->mask;
					compVec.erase(compVec.begin() + i);
					return;
				}
			}
		}
	}
};