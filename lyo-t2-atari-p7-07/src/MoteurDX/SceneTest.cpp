#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"

void SceneTest::OnInitialize()
{
	// 1
	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent(entity1, COMPONENT_TRANSFORM | COMPONENT_MESH);
	MeshComponent* mesh = static_cast<MeshComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->tab_components[Mesh_index]);
	TransformComponent* transform = static_cast<TransformComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->tab_components[Transform_index]);

	mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
	transform->m_transform.Scale(1.0f, 1.0f, 1.0f);

	// 2
	Entity* entity2 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent(entity2, COMPONENT_TRANSFORM | COMPONENT_MESH);
	mesh = static_cast<MeshComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity2->tab_index]->tab_components[Mesh_index]);
	transform = static_cast<TransformComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity2->tab_index]->tab_components[Transform_index]);

	mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(4.0f, 1.0f, 3.0f, 5.0f, 2.0f, 0.0f);
	transform->m_transform.Move(0.0f, 2.0f, 2.0f);
}

void SceneTest::OnUpdate()
{
	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
		{
			continue;
		}
		TransformComponent* transform = static_cast<TransformComponent*>(mpGameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->tab_components[Transform_index]);
		transform->m_transform.UpdateMatrix();
	}
}

void SceneTest::OnClose()
{

}
