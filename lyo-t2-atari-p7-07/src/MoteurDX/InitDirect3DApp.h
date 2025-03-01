#pragma once

#include "WindowDX.h";
//#include "Camera.h";
#include "Scene.h";
#include "HealthSystem.h"
#include "AttackSystem.h"

//class GameManager;
class MeshFactory;
class Camera;
class ColliderManager;
//class HealthSystem;
//class AttackSystem;

class TextureManager;

//struct AABB {
//	DirectX::XMFLOAT3 min;
//	DirectX::XMFLOAT3 max;
//};

class InitDirect3DApp : public WindowDX
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	bool Initialize();
	bool InitTexture();
	void Update() override;
	void UpdatePhysics();
	void UpdateTimer();
	void Draw() override;

	// __ Game loop __ 

	//void HandleInput();
	//AABB GetAABB(const Transform& transform);
	//bool CheckCollision(const AABB& a, const AABB& b);
	//bool AABBIntersect(const TransformComponent& a, const TransformComponent& b);

	void Render();
	void CreatePipelineState();

	void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
	void SetScene(Scene* scene) { m_scene = scene; }
	//void SetCamera(Camera* camView) { m_Camera = camView; }


	EntityManager* GetEntityManager() { return m_entityManager; }
	MeshFactory* GetFactory() { return m_meshFactory; }

	CameraComponent* GetMainView() { return m_mainView; }

	friend class Scene;

private:
	D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;

	CameraComponent* m_mainView;
	HealthSystem m_healthSystem;
	AttackSystem* m_attackSystem;

	MeshFactory* m_meshFactory;
	EntityManager* m_entityManager;
	TextureManager* m_textureManager;
	ColliderManager* m_colliderManager;

	Scene* m_scene;

	float m_deltaTime = 0.0f; //    /!\/!\/!\/!\    A UTILISER DANS LA BOUCLE DE JEU    /!\/!\/!\/!\/
	DWORD m_lastTime = 0;

	// Textures:
	ComPtr<ID3D12Resource> m_texture;
};

