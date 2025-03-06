#pragma once

#include "WindowDX.h"
#include "Scene.h"
#include "LightSystem.h"

class MeshFactory;

class CameraSystem;
class Attackystem;
class HealthSystem;

class ColliderManager;
class ParticleManager;
class EnnemyManager;
class MovementManager;
class LifeTimeManager;
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
	void SetScene(Scene* scene) { mp_scene = scene; }

	EntityManager* GetEntityManager() { return mp_entityManager; }
	MeshFactory* GetFactory() { return mp_meshFactory; }
	ParticleManager* GetParticleManager() { return mp_particleManager; }
	CameraSystem* GetCameraSystem() { return mp_cameraManager; }
	EnnemyManager* GetEnnemyManager() { return mp_ennemyManager; }

	friend class Scene;

private:
	D3D12_DEPTH_STENCIL_DESC m_depthStencilDesc;

	MeshFactory* mp_meshFactory;

	EntityManager* mp_entityManager;
	ColliderManager* mp_colliderManager;
	EnnemyManager* mp_ennemyManager;
	ParticleManager* mp_particleManager;
	TextureManager* mp_textureManager;
	MovementManager* mp_movementManager;
	LifeTimeManager* mp_lifeTimeManager;

	HealthSystem* mp_healthSystem;
	AttackSystem* mp_attackSystem;
	CameraSystem* mp_cameraManager;
	LightSystem* mp_lightSystem;

	Scene* mp_scene;

	float m_deltaTime = 0.0f;
	DWORD m_lastTime = 0;

	ComPtr<ID3D12Resource> m_texture;

	bool m_gameIsPaused;

	ComPtr<ID3D12Resource> m_passConstantBuffer;
	PassConstants* mp_mappedPassCB = nullptr;
};

