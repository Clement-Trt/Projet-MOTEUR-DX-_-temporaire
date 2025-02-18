#pragma once

class GameManager;
class EntityManager;
class Movement;

class Scene
{
protected:

	// Pointeur vers GM
	GameManager* mpGameManager;
	EntityManager* mpEntityManager;

	// Pointeur vers chaque System
	Movement* mMvmt;
public:

	//Scene();

	// Initialisation des calsses System avec le GM
	void Initialize(GameManager* gameManager);
	virtual void OnInitialize() = 0;

	void Update();
	virtual void OnUpdate() = 0;

	void Close();
	virtual void OnClose() = 0;


	//void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	friend class GameManager;
};

