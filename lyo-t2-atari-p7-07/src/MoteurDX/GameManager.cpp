#include "pch.h"
//
//#include "GameManager.h"
//
//#include <iostream>
//#include "EntityManager.h"
//#include "Movement.h"
//#include "Scene.h"
//
//void GameManager::Initialize(Scene* scene)
//{
//	mp_entityManager = new EntityManager();
//	SetScene(scene);
//	m_scene->Initialize(this);
//	m_scene->OnInitialize();
//}
//
//void GameManager::Run()
//{
//	//sf::Clock clock; REMPLACER PAR SYSTEME DE TIMER
//
//		//SetDeltaTime(m_deltaTime); // CHANGER PAR SYSTEME DE TIMER
//
//		//HandleInput(); // AJOUTER SYSTEME DE GESTION D'INPUT
//
//	Update();
//
//	//Draw(); // AJOUTER SYSTEME DE DESSIN DIRECTX
//}
//
//void GameManager::Update()
//{
//
//
//	//if (mp_entityManager->GetEntityTab()[0] != nullptr)
//	//{
//	//	// Update
//	//	m_scene->OnUpdate();
//
//
//	//	// COLLISIONS
//
//	//}
//
//
//	//// DESTROY ENTITIES
//	//for (auto& entityToDestroy : mp_entityManager->GetToDestroyTab())
//	//{
//	//	mp_entityManager->DestroyEntity(entityToDestroy);
//	//}
//	//mp_entityManager->GetToDestroyTab().clear();
//
//	//// ADD ENTITIES
//	//for (auto& entityToAdd : mp_entityManager->GetEntityToAddTab())
//	//{
//	//	mp_entityManager->AddEntityToTab(entityToAdd, mp_entityManager->GetComponentToAddTab()[entityToAdd->tab_index]);
//	//}
//	//mp_entityManager->GetEntityToAddTab().clear();
//	//mp_entityManager->GetComponentToAddTab().clear();
//	//mp_entityManager->ResetEntitiesToAdd();
//
//}
//
//void GameManager::Draw()
//{
//	// AJOUTER SYSTEME DE DESSIN DIRECTX
//}
