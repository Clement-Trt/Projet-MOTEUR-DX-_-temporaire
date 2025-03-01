#pragma once

#include "WindowDX.h";
#include "Scene.h";

class MeshFactory;
class Camera;
class ColliderManager;
class ParticleManager;
class EnnemyManager;
class MovementManager;
class CameraSystem;
class Attackystem;
class HealthSystem;

class TextureManager;

class InitDirect3DApp : public WindowDX
{
public:
	InitDirect3DApp(HINSTANCE hInstance);
	~InitDirect3DApp();

	bool Initialize();
	bool InitTexture();
	void Update() override;
	void UpdatePhysics();
	void UpdateTimer();
	void Draw() override;


	void Render();
	void CreatePipelineState();

	void SetDeltaTime(float deltaTime) { m_deltaTime = deltaTime; }
	void SetScene(Scene* scene) { m_scene = scene; }


	EntityManager* GetEntityManager() { return m_entityManager; }
	MeshFactory* GetFactory() { return m_meshFactory; }
	ParticleManager* GetParticleManager() { return m_particleManager; }

	CameraComponent* GetMainView() { return m_mainView; }

	friend class Scene;

private:
	D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;

	MeshFactory* m_meshFactory;

	EntityManager* m_entityManager;
	CameraComponent* m_mainView;

	ColliderManager* m_colliderManager;
	EnnemyManager* m_ennemyManager;
	ParticleManager* m_particleManager;
	TextureManager* m_textureManager;
	MovementManager* m_movementManager;

	HealthSystem* m_healthSystem;
	AttackSystem* m_attackSystem;
	CameraSystem* m_cameraManager;

	Scene* m_scene;

	float m_deltaTime = 0.0f; //   PAS UTILISE ?
	DWORD m_lastTime = 0;

	// Textures:
	ComPtr<ID3D12Resource> m_texture;
};

