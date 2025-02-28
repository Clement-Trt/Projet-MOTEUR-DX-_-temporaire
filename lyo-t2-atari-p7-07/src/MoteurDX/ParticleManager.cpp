#include "pch.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
}

void ParticleManager::InitParticleManager(EntityManager* entityManager, InitDirect3DApp* app)
{
	m_entityManager = entityManager;
	m_gameManager = app;
}

void ParticleManager::Update()
{
	for (auto& entity : m_entityManager->GetEntityTab())
	{
		if (entity == nullptr)
			continue;// Msg d'erreur ?

		// Transform & Collider Components
		TransformComponent* transform1 = nullptr;
		ColliderComponent* collider1 = nullptr;
		HealthComponent* health1 = nullptr;
		AttackComponent* attack1 = nullptr;
		for (auto* component : m_entityManager->GetComponentsTab()[entity->tab_index]->vec_components)
		{
			if (component->ID == Particle_ID)
			{
			}
		}
	}
	// Gere les particule creer dans la liste
	// Update les mouvements et gere les deletes
}

void ParticleManager::CreateDefaultBlock(float startPosX, float startPosY, float startPosZ, float size,  float speedX, float speedY, float speedZ)
{
	Entity* newIceBlock = m_entityManager->CreateEntity();
	m_entityManager->AddComponent<TransformComponent>(newIceBlock);
	m_entityManager->AddComponent<MeshComponent>(newIceBlock);
	m_entityManager->AddComponent<particleComponent>(newIceBlock);
	m_entityManager->AddComponent<VelocityComponent>(newIceBlock);

	for (auto& comp : m_gameManager->GetEntityManager()->GetComponentToAddTab()[newIceBlock->tab_index]->vec_components)
	{
		if (comp->ID == Mesh_ID)
		{
			MeshComponent* mesh = static_cast<MeshComponent*>(comp);
			mesh->m_cubeMesh = m_gameManager->GetFactory()->CreateCube();
			mesh->textureID = L"FireTexture";
		}
		if (comp->ID == Transform_ID)
		{
			TransformComponent* transform = static_cast<TransformComponent*>(comp);
			transform->m_transform.Scale(size, size, size);
			transform->m_transform.Move(startPosZ, startPosX, startPosY);
		}
		if (comp->ID == Particle_ID)
		{
			particleComponent* particle = static_cast<particleComponent*>(comp);
		}
		if (comp->ID == Velocity_ID)
		{
			VelocityComponent* vel = static_cast<VelocityComponent*>(comp);
			vel->vx = speedX;
			vel->vy = speedY;
			vel->vz = speedZ;
		}
	}
}

void ParticleManager::Explosion(float startPosX, float startPosY, float startPosZ)
{
	MakeEffect(startPosX, startPosY, startPosZ, 5, 10, -1, 1, 2, 5);
}

void ParticleManager::MakeEffect(float startPosX, float startPosY, float startPosZ, int _minNbPart, int _maxNbPart, int _minSpeed, int _maxSpeed, int minSize, int maxSize)
{
	int randSplashNb = std::rand() % (_maxNbPart - _minNbPart + 1) + _minNbPart;
	wchar_t buffer[256];
	for (int i = 0; i < randSplashNb; i++)
	{
		float randSize = minSize + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (maxSize - minSize);
		float randSpeedX = _minSpeed + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (_maxSpeed - _minSpeed);
		float randSpeedY = _minSpeed + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (_maxSpeed - _minSpeed);
		float randSpeedZ = _minSpeed + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (_maxSpeed - _minSpeed);

		CreateDefaultBlock(startPosX, startPosY, startPosZ, randSize, randSpeedX, randSpeedY, randSpeedZ);
		/*swprintf_s(buffer, 256, L"NewParticle speedx: %d\r\n", randSpeedX); ne prend pas en compte les float
		OutputDebugString(buffer);*/
	}

	
	swprintf_s(buffer, 256, L"NB new Particles: %d\r\n", randSplashNb);
	OutputDebugString(buffer);
}

void ParticleManager::CreateParticle(float size, float speed)
{
	//Particle* newParticle = new Particle(_radius, _position.x, _position.y, _speed, _spritePath);
	//particleList.push_back(newParticle);
}