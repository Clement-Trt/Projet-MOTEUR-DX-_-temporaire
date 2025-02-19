#include "pch.h"

#include <iostream>
#include "Scene.h"
#include "InitDirect3DApp.h"
//#include "EntityManager.h"
//#include "GameManager.h"
#include "Movement.h"

void Scene::Initialize(InitDirect3DApp* gameManager)
{
	mpGameManager = gameManager;

	mpEntityManager = mpGameManager->m_entityManager;

	mMvmt = new Movement;
	mMvmt->Initialize(mpGameManager);


}

void Scene::Update()
{

}

void Scene::Close()
{
}
