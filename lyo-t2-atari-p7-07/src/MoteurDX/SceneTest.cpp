#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"

void SceneTest::OnInitialize()
{

	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent(entity1, COMPONENT_TRANSFORM | COMPONENT_MESH);
	//mpEntityManager->GetComponentToAddTab()[entity1->tab_index].
	MeshComponent* mesh = static_cast<MeshComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->tab_components[Mesh_index]);
	TransformComponent* transform = static_cast<TransformComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->tab_components[Transform_index]);

	mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(entity1,1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
	transform->m_transform.Scale(1.0f, 1.0f, 1.0f);
	transform->m_transform.Move(0.0f, 2.0f, 0.0f);
}

void SceneTest::OnUpdate()
{
	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
		{
			break;
		}
		TransformComponent* transform = static_cast<TransformComponent*>(mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity->tab_index]->tab_components[Transform_index]);
		transform->m_transform.UpdateMatrix();
	}
}

void SceneTest::OnClose()
{

}
