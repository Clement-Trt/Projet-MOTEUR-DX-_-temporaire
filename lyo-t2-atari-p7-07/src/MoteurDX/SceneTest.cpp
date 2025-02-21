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

	mpEntityManager->AddComponent<TransformComponent>(entity1);
	mpEntityManager->AddComponent<MeshComponent>(entity1);
	mpEntityManager->AddComponent<VelocityComponent>(entity1);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(1.0f, 1.0f, 1.0f);
		}
	}

	// 2
	Entity* entity2 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(entity2);
	mpEntityManager->AddComponent<MeshComponent>(entity2);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity2->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
		}
		if (component->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(component);
			transform->m_transform.Scale(3.0f, 1.0f, 1.0f);
			transform->m_transform.Move(3.0f, 3.0f, 3.0f);
		}
	}

	compteur = 50;
	compteur2 = 150;

}

void SceneTest::OnUpdate()
{
	for (auto& entity : mpEntityManager->GetEntityTab())
	{
		if (entity == nullptr)
		{
			continue;
		}

		// Update de positions
		if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY))
		{
			TransformComponent* transform = nullptr;
			VelocityComponent* velComponent = nullptr;
			

			for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
			{
				if (component->ID == Transform_ID)
				{
					transform = static_cast<TransformComponent*>(component);
				}
				if (component->ID == Velocity_ID)
				{
					velComponent = static_cast<VelocityComponent*>(component);
				}
			}
			
			compteur--;
			if (compteur <= 0)
			{
				compteur = 50;
				mMvmt->SetVelocity(velComponent, -velComponent->vz, velComponent->vx, velComponent->vy);
			}

			if (transform && velComponent)
			{
				transform->m_transform.Move(velComponent->vz, velComponent->vx, velComponent->vy);
				transform->m_transform.UpdateMatrix();
			}
		}

		//// Update finale
		//for (auto* component : mpGameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
		//{
		//	if (component->ID == Transform_ID)
		//	{
		//		TransformComponent* transform = static_cast<TransformComponent*>(component);
		//		transform->m_transform.UpdateMatrix();
		//	}
		//}
	}


	/*compteur2--;
	if (compteur2 <= 0)
	{*/
		/*compteur2 = 150;

		Entity* newEntity = mpEntityManager->CreateEntity();

		mpEntityManager->AddComponent<TransformComponent>(newEntity);
		mpEntityManager->AddComponent<MeshComponent>(newEntity);
		mpEntityManager->AddComponent<VelocityComponent>(newEntity);

		for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[newEntity->tab_index]->vec_components)
		{
			if (component->ID == Mesh_ID)
			{
				MeshComponent* mesh = static_cast<MeshComponent*>(component);
				mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube();
			}
			if (component->ID == Transform_ID)
			{
				TransformComponent* transform = static_cast<TransformComponent*>(component);
				transform->m_transform.Scale(0.1f, 0.1f, 0.1f);
			}
			if (component->ID == Velocity_ID)
			{
				VelocityComponent* velComponent = static_cast<VelocityComponent*>(component);
				mMvmt->SetVelocity(velComponent, 0.5f, 0.0f, 0.0f);
			}
		}*/
	//}
}

void SceneTest::OnClose()
{

}
