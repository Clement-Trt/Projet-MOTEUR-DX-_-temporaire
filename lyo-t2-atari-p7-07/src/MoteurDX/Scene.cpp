#include "pch.h"

#include <iostream>
#include "Scene.h"
#include "EntityManager.h"
#include "GameManager.h"
#include "Movement.h"

void Scene::Initialize(GameManager* gameManager)
{
	mpGameManager = gameManager;

	mpEntityManager = mpGameManager->mEM;

	mMvmt = new Movement;
	mMvmt->Initialize(mpGameManager);


}

void Scene::Update()
{

}

void Scene::Close()
{
}
