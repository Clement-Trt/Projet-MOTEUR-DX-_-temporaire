#pragma once

//#include "EntityManager.h"

class EntityManager;
//class Movement;

class Scene;

class GameManager
{
protected:

	EntityManager* mEM;
	//Movement* mMvmt;

	Scene* mScene;

	float mDeltaTime;

public:

	//GameManager();
	void Initialize(Scene* scene);

	// __ Game loop __ 
	void Run();
	//void UpdatePhysics();
	void HandleInput();
	void Update();
	void Draw();

	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
	void SetScene(Scene* scene) { mScene = scene; }

	EntityManager* GetEntityManager() { return mEM; }

	friend class Scene;
};

