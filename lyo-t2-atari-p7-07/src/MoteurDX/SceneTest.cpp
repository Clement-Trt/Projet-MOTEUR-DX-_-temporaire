#include "pch.h"

#include "SceneTest.h"

#include <iostream>

#include "EntityManager.h"
#include "Movement.h"
#include "InitDirect3DApp.h"
#include "Camera.h"
#include "InputManager.h"

void SceneTest::OnInitialize()
{
	// 1
	Entity* entity1 = mpEntityManager->CreateEntity();

	mpEntityManager->AddComponent<TransformComponent>(entity1);
	mpEntityManager->AddComponent<MeshComponent>(entity1);
	mpEntityManager->AddComponent<VelocityComponent>(entity1);
	mpEntityManager->AddComponent<CameraComponent>(entity1);

	for (auto& component : mpGameManager->GetEntityManager()->GetComponentToAddTab()[entity1->tab_index]->vec_components)
	{
		if (component->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(component);
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(1.0f, 1.0f, 1.0f, 0.0f, 2.0f, 0.0f);
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
			mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(1.0f, 1.0f, 1.0f, 3.0f, 3.0f, 3.0f);
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
		if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_CAMERA))
		{
			TransformComponent* transform = nullptr;
			CameraComponent* camComponent = nullptr;


			for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
			{
				if (component->ID == Transform_ID)
				{
					transform = static_cast<TransformComponent*>(component);
				}
				if (component->ID == Camera_ID)
				{
					camComponent = static_cast<CameraComponent*>(component);
				}
			}


			//// Mettez a jour la souris en passant le handle de la fenetre
			//InputManager::UpdateMouse(GetActiveWindow());

			//// Recuperer le deplacement de la souris
			//int deltaX = InputManager::GetMouseDeltaX();
			//int deltaY = InputManager::GetMouseDeltaY();

			//// Sensibilite de la souris
			//const float sensitivity = 0.005f;
			////if (InputManager::GetKeyIsPressed(MK_LBUTTON))
			////{
			////	// Mettre a jour la rotation de la camera en fonction du delta
			////	transform->m_transform.Rotation(0.0f, -deltaY * sensitivity, deltaX * sensitivity);
			////}

			//if (InputManager::GetKeyIsPressed('Q')) transform->m_transform.Move(0.0f, 0.1f, 0.0f);
			//if (InputManager::GetKeyIsPressed('D')) transform->m_transform.Move(0.0f, -0.1f, 0.0f);

			//if (InputManager::GetKeyIsPressed('Z')) transform->m_transform.Move(-0.1f, 0.0f, 0.0f);
			//if (InputManager::GetKeyIsPressed('S')) transform->m_transform.Move(0.1f, 0.0f, 0.0f);

			//if (InputManager::GetKeyIsPressed('A')) transform->m_transform.Move(0.0f, 0.0f, 0.1f);
			//if (InputManager::GetKeyIsPressed('E')) transform->m_transform.Move(0.0f, 0.0f, -0.1f);

			//if (transform != nullptr && camComponent != nullptr)
			//{
			//	//transform->m_transform
			//	if (transform->m_transform.GetPositionX() && transform->m_transform.GetPositionY() && transform->m_transform.GetPositionZ())
			//		camComponent->m_camera->Move(transform->m_transform.GetPositionX(), transform->m_transform.GetPositionY(), transform->m_transform.GetPositionZ());
			//	//camComponent->m_camera->Move(transform->m_transform.GetPositionX(), transform->m_transform.GetPositionY(), transform->m_transform.GetPositionZ());
			//	//camComponent->m_camera->Move(transform->m_transform.GetPositionX(), transform->m_transform.GetPositionY(), transform->m_transform.GetPositionZ());
			//}
		}
	}



	//for (auto& entity : mpEntityManager->GetEntityTab())
	//{
	//	if (entity == nullptr)
	//	{
	//		continue;
	//	}

	//	// Update de positions
	//	if (mpEntityManager->HasComponent(entity, COMPONENT_TRANSFORM | COMPONENT_VELOCITY))
	//	{
	//		TransformComponent* transform = nullptr;
	//		VelocityComponent* velComponent = nullptr;
	//		

	//		for (auto* component : mpEntityManager->GetComponentsTab()[entity->tab_index]->vec_components)
	//		{
	//			if (component->ID == Transform_ID)
	//			{
	//				transform = static_cast<TransformComponent*>(component);
	//			}
	//			if (component->ID == Velocity_ID)
	//			{
	//				velComponent = static_cast<VelocityComponent*>(component);
	//			}
	//		}
	//		
	//		compteur--;
	//		if (compteur <= 0)
	//		{
	//			compteur = 50;
	//			mMvmt->SetVelocity(velComponent, -velComponent->vz, velComponent->vx, velComponent->vy);
	//		}

	//		if (transform && velComponent)
	//		{
	//			transform->m_transform.Move(velComponent->vz, velComponent->vx, velComponent->vy);
	//			transform->m_transform.UpdateMatrix();
	//		}
	//	}

		//// Update finale
		//for (auto* component : mpGameManager->GetEntityManager()->GetComponentsTab()[entity->tab_index]->vec_components)
		//{
		//	if (component->ID == Transform_ID)
		//	{
		//		TransformComponent* transform = static_cast<TransformComponent*>(component);
		//		transform->m_transform.UpdateMatrix();
		//	}
		//}



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
		mesh->m_cubeMesh = mpGameManager->GetFactory()->CreateCube(1.0f, 1.0f, 10.0f, 0.0f, 2.0f, 0.0f);
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
