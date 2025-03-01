#pragma once

class GameManager;
class EntityManager;
class Movement;
class AttackSystem;
class InitDirect3DApp;

class Scene
{
protected:

	// Pointeur vers GM
	InitDirect3DApp* m_gameManager;
	EntityManager* mpEntityManager;

	// Pointeur vers chaque System
	//Movement* mMvmt;
	AttackSystem* mAttack;
public:

	//Scene();

	// Initialisation des calsses System avec le GM
	void Initialize(InitDirect3DApp* gameManager);
	virtual void OnInitialize() = 0;

	void Update();
	virtual void OnUpdate() = 0;

	void Close();
	virtual void OnClose() = 0;


	//void SetGameManager(GameManager* pGameManager) { mpGameManager = pGameManager; }
	friend class GameManager;
};

