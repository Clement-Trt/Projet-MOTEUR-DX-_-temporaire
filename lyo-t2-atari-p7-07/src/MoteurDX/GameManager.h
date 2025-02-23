#pragma once

class EntityManager;
//class Movement;

class Scene;

class GameManager
{
protected:

	//Movement* mMvmt;
	EntityManager* mEM;

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

	EntityManager* GetEntityManager() { return mEM; }

	friend class Scene;
};

