#include "pch.h"

#include <iostream>
#include "Scene.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"
//#include "GameManager.h"
#include "Movement.h"

void Scene::Initialize(InitDirect3DApp* gameManager)
{
	m_gameManager = gameManager;

	mpEntityManager = m_gameManager->m_entityManager;

	/*mMvmt = new Movement;
	mMvmt->InitMovementManager(mpGameManager);*/
	//mAttack = new AttackSystem;
	//mAttack->Initialize(mpGameManager);
}

void Scene::Update()
{

}

void Scene::Close()
{
}
