#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "GameManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"

void SceneTest::OnInitialize()
{

	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent(entity1, COMPONENT_TRANSFORM | COMPONENT_MESH);
	//mpEntityManager->GetComponentToAddTab()[entity1->tab_index].
	MeshComponent* mesh = static_cast<MeshComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->tab_components[Mesh_index]);

	//mesh->m_cubeMesh = CreateCubeMesh();
}

void SceneTest::OnUpdate()
{
	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
		{
			break;
		}
	}
}

void SceneTest::OnClose()
{

}
