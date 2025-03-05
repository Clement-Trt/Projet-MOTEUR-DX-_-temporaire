#pragma once

class EntityManager;
class Scene;

class GameManager
{
protected:

	//Movement* mMvmt;
	EntityManager* mp_entityManager;

	Scene* m_scene;

	float m_deltaTime;

public:

	//GameManager();
	void Initialize(Scene* scene);

	// __ Game loop __ 
	void Run();
	//void UpdatePhysics();
	void HandleInput();
	void Update();
	void Draw();

	void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
	void SetScene(Scene* scene) { m_scene = scene; }

	EntityManager* GetEntityManager() { return mp_entityManager; }

	friend class Scene;
};

