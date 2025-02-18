#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "GameManager.h"
#include "Movement.h"

void SceneTest::OnInitialize()
{

	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent(entity1, COMPONENT_TRANSFORM | COMPONENT_MESH);
	//mpEntityManager->GetComponentToAddTab()[entity1->tab_index].


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
