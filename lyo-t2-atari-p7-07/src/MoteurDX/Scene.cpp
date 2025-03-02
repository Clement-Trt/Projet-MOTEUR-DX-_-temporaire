#include "pch.h"

#include <iostream>
#include "Scene.h"
#include "InitDirect3DApp.h"

void Scene::Initialize(InitDirect3DApp* gameManager)
{
	m_gameManager = gameManager;

	mpEntityManager = m_gameManager->m_entityManager;

	m_camera = m_gameManager->GetCameraSystem();
}

void Scene::Update()
{

}

void Scene::Close()
{
}
