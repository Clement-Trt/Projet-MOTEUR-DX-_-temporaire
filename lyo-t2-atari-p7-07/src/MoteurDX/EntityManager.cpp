#include "pch.h"

#include "EntityManager.h"

EntityManager::EntityManager()
{
	for (auto& ptr : tab_Components)
		ptr = nullptr;
}

EntityManager::~EntityManager()
{
	for (auto& entity : tab_entity)
	{
		delete entity;
		entity = nullptr;
	}

	//for (auto& entityComponents : tab_Components)
	//{
	//	if (entityComponents)
	//	{
	//		/*for (auto* component : tab_Components[entityComponents->tab_index]->vec_components)
	//		{
	//			delete component;
	//		}*/
	//		//tab_Components[entityComponents->tab_index]->vec_components.clear();

	//		delete entityComponents;
	//		entityComponents = nullptr;
	//	}
	//}

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
	/*for (auto& component : tab_compToAdd)
	{
		delete component;
		component = nullptr;
	}
	tab_compToAdd.clear();*/
}

Entity* EntityManager::CreateEntity() {

	Entity* entity = new Entity;

	//int newid = entitiesToAddID * -1;
	/*if (newid > 64000)
		return nullptr;*/
	entity->tab_index = entitiesToAddID * -1;

	entity->id = entitiesToAddID - 1;
	tab_entitiesToAdd.push_back(entity);

	auto newComponents = std::make_unique<EntityComponents>();
	newComponents->mask = COMPONENT_NONE;
	newComponents->tab_index = entitiesToAddID;

	tab_compToAdd.push_back(std::move(newComponents));

	entitiesToAddID--;
	return entity;
}

//void EntityManager::AddEntityToTab(Entity* entity, std::unique_ptr<EntityComponents> components)
//{
//	entity->tab_index = entityNb;
//	entity->id = entityNb + 1;
//	tab_entity[entity->tab_index] = entity; // Ajout de l'entity dans le tableau d'entities
//
//	components->tab_index = entityNb;
//	tab_Components[entityNb] = components.get(); // Ajout des components (vides) de l'entity dans la case correspondant du tableau de composants
//
//	entityNb++;
//}
//
//void EntityManager::DestroyEntity(Entity* entity/*, EntityComponents* components*/) {
//	if (entity->tab_index > 64000)
//		return;
//	
//	
//	int index = entity->tab_index;
//	int lastIndex = entityNb - 1;
//
//	if (index > lastIndex) {
//		std::cerr << "Index of entity to destroy invalid." << std::endl;
//		return;
//	}
//	if (index > 64000 || lastIndex > 64000) {
//		std::cerr << "Index of entity to destroy invalid." << std::endl;
//		return;
//	}
//
//	tab_entity[lastIndex]->tab_index = index;
//	*tab_entity[index] = *tab_entity[lastIndex];
//
//	tab_Components[lastIndex]->tab_index = index;
//	//*tab_Components[index] = *tab_Components[lastIndex];
//	//std::swap(tab_Components[index], tab_Components[lastIndex]);
//	tab_Components[index] = std::move(tab_Components[lastIndex]);
//	
//	delete tab_entity[lastIndex];
//	tab_entity[lastIndex] = nullptr;
//
//	
//	delete tab_Components[lastIndex];
//	tab_Components[lastIndex] = nullptr;
//	--entityNb;
//}

void EntityManager::AddEntityToTab(Entity* entity, std::unique_ptr<EntityComponents> components)
{
	assert(components && "Erreur : tentative d'ajout d'un component nullptr !");

	entity->tab_index = entityNb;
	entity->id = entityNb + 1;
	tab_entity[entity->tab_index] = entity; // Ajout de l'entity dans le tableau d'entities

	components->tab_index = entityNb;
	//tab_Components[entityNb] = std::move(components); // Transfert unique_ptr
	assert(tab_Components[entityNb] && "tab_Components[index] est nullptr !");
	tab_Components.at(entityNb) = std::move(components);

	entityNb++;
}

void EntityManager::DestroyEntity(Entity* entity) {
	if (entity->tab_index > 64000)
		return;

	int index = entity->tab_index;
	int lastIndex = entityNb - 1;

	if (index > lastIndex) {
		std::cerr << "Index of entity to destroy invalid." << std::endl;
		return;
	}

	if (index > 64000 || lastIndex > 64000) {
		std::cerr << "Index of entity to destroy invalid." << std::endl;
		return;
	}

	assert(tab_Components[index] && "tab_Components[index] est nullptr !");

	// Déplacement des entités et des composants
	tab_entity[index] = tab_entity[lastIndex];  // Swap avec la dernière entité
	tab_Components[index] = std::move(tab_Components[lastIndex]);  // Swap avec le dernier composant

	// Réinitialisation de l'entité et des composants déplacés
	tab_entity[lastIndex] = nullptr;
	tab_Components[lastIndex] = nullptr;  // La mémoire est gérée automatiquement ici

	--entityNb;  // Décrémentation du compteur d'entités
}


void EntityManager::ToDestroy(Entity* entity)
{
	for (auto& entityInTab : tab_toDestroy)
	{
		if (entityInTab == entity)
		{
			wchar_t buffer[256];
			swprintf_s(buffer, 256, L"Entity to destroy is already in to destroy list\r\n");
			OutputDebugString(buffer);
			return;
		}
	}
	tab_toDestroy.push_back(entity);
}

bool EntityManager::HasComponent(Entity* entity, ComponentMask componentMask) const {

	assert(tab_Components[entity->tab_index] && "tab_Components[index] est nullptr !");
	return (tab_Components[entity->tab_index]->mask & componentMask) == componentMask;
}


