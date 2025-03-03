#include "pch.h"

#include "SandBoxScene.h"

#include <iostream>

#include "EntityManager.h"
#include "GameManager.h"

void SandBoxScene::OnInitialize()
{
	//Entity* entity1 = mpEntityManager->CreateEntity();
	//Entity* entity2 = mpEntityManager->CreateEntity();
	//Entity* entity3 = mpEntityManager->CreateEntity();
	//Entity* entity4 = mpEntityManager->CreateEntity();

	//mpEntityManager->AddComponent(entity1, COMPONENT_VELOCITY | COMPONENT_TRANSFORM);
	//mMvmt->SetVelocity(entity1, 1.0, 1.0, 1.5);


	//mpEntityManager->AddComponent(entity2, COMPONENT_VELOCITY | COMPONENT_TRANSFORM);
	//mMvmt->SetVelocity(entity2, 1.0, 2.0, 2.5);


	//mpEntityManager->AddComponent(entity3, COMPONENT_VELOCITY | COMPONENT_TRANSFORM);
	//mMvmt->SetVelocity(entity3, 1.0, 5.0, 5.5);



	//mpEntityManager->AddComponent(entity4, COMPONENT_VELOCITY | COMPONENT_TRANSFORM);
	//mMvmt->SetVelocity(entity4, 1.0, 7.0, 7.5);
}

void SandBoxScene::OnUpdate()
{
	//for (auto& entity : mpEntityManager->GetEntityTab())
	//{
	//	if (entity == nullptr)
	//	{
	//		break;
	//	}


	//	TransformComponent* transform = static_cast<TransformComponent*>(mpEntityManager->GetComponentsTab()[entity->tab_index]->tab_components[Transform_index]);
	//	MeshComponent* mesh = static_cast<MeshComponent*>(mpEntityManager->GetComponentsTab()[entity->tab_index]->tab_components[Mesh_index]);
	//	VelocityComponent* velocity = static_cast<VelocityComponent*>(mpEntityManager->GetComponentsTab()[entity->tab_index]->tab_components[Velocity_index]);

	//	std::cout << "\n ------------------------------------------------------------------------------------------- \n";
	//	std::cout << "Entite " << entity->id << " position d'origine : ("
	//		<< transform->m_transform.GetPositionX() << ", " << transform->m_transform.GetPositionY() << ")\n";


	//	if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY)) {

	//		for (auto& component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
	//		{
	//			//if (component)
	//		}

	//		transform->m_transform.Move(velocity->vz, velocity->vx, velocity->vy);

	//		std::cout << "Entite " << entity->id << " nouvelle position: ("
	//			<< transform->m_transform.GetPositionX() << ", " << transform->m_transform.GetPositionY() << ") avec : vx = " << velocity->vx << " et vy = " << velocity->vy << std::endl;
	//	}
	//	std::cout << "\n ------------------------------------------------------------------------------------------- \n";
	//}

	//for (auto entity : mpEntityManager->GetEntityTab())
	//{
	//	if (entity == nullptr)
	//	{
	//		break;
	//	}
	//	std::cout << "\n\n ------------------------------------------------------------------------------------------- \n";
	//	std::cout << "Entity : " << entity->id << std::endl;
	//}
}

void SandBoxScene::OnClose()
{
}
