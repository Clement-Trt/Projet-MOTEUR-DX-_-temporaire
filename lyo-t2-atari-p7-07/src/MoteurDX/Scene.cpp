#include "pch.h"

#include <iostream>
#include "Scene.h"
#include "InitDirect3DApp.h"

void Scene::Initialize(InitDirect3DApp* gameManager)
{
	mp_gameManager = gameManager;

	mpEntityManager = mp_gameManager->mp_entityManager;

	mp_camera = mp_gameManager->GetCameraSystem();
}

void Scene::Update()
{

}

void Scene::Close()
{
}
