#pragma once

#include "Transform.h"
#include "MeshFactory.h"

class Entity;

using namespace Microsoft::WRL;

using ComponentMask = uint32_t;

enum ComponentType {
	COMPONENT_NONE = 0,
	COMPONENT_CAMERA = 1 << 0,
	COMPONENT_MESH = 1 << 1,
	COMPONENT_TRANSFORM = 1 << 2,
	COMPONENT_VELOCITY = 1 << 3,
	COMPONENT_HEALTH = 1 << 4,
	COMPONENT_ATTACK = 1 << 5,
	COMPONENT_COLLIDER = 1 << 6,
	COMPONENT_PARTICLE = 1 << 7,
	COMPONENT_ENNEMY = 1 << 8,
	COMPONENT_PLAYER = 1 << 9,
	COMPONENT_LIGHT = 1 << 10,
	COMPONENT_LIFETIME = 1 << 11,
	COMPONENT_HIGHLIGHT = 1 << 12,
	COMPONENT_SCENEOBJECT = 1 << 13,
};

enum ComponentIndex
{
	Camera_index,
	Mesh_index,
	Transform_index,
	Velocity_index,
	Health_index,
	Attack_index,
	Collider_index,
	Particle_index,
	Ennemy_index,
	Player_index,
	Light_index,
	LifeTime_index,
	Highlight_index,
	SceneObject_index,
};

enum ComponentID
{
	Camera_ID,
	Mesh_ID,
	Transform_ID,
	Velocity_ID,
	Health_ID,
	Attack_ID,
	Collider_ID,
	Particle_ID,
	Ennemy_ID,
	Player_ID,
	Light_ID,
	LifeTime_ID,
	Highlight_ID,
	SceneObject_ID,

	TotalComponentsNumber
};

struct GeometryMesh
{
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	ComPtr<ID3D12Resource> m_indexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	// Nombre d'indices pour le rendu
	UINT m_meshIndex;
};

struct Mesh
{
	// typeOf Geometry
	GeometryMesh m_geometryMesh;

	// Buffer de constantes (matrices, couleurs, etc.)
	ComPtr<ID3D12Resource> m_constantBuffer;

	void* m_mappedData = nullptr;
};


struct Component
{
	Component(int id, ComponentMask m) : ID(id), mask(m) {}
	int ID = -1;
	ComponentMask mask = 0;
};

struct CameraComponent : public Component
{
	CameraComponent() : Component(Camera_ID, COMPONENT_CAMERA) {}
	Transform m_cameraTransform;
	DirectX::XMMATRIX m_cameraView;
};

struct MeshComponent : public Component
{
	MeshComponent() : Component(Mesh_ID, COMPONENT_MESH) {}
	Mesh* m_cubeMesh;
	std::wstring m_textureID = L"";
};

struct ColliderComponent : public Component
{
	ColliderComponent() : Component(Collider_ID, COMPONENT_COLLIDER) {}
	bool m_isColliding = false;
	bool m_isSolid = false;
	bool m_isDynamic = false;
	bool m_destructable = false;
	bool m_isDestructable = false;
	bool m_isDestroyed = false;
};

struct ParticleComponent : public Component 
{
	ParticleComponent() : Component(Particle_ID, COMPONENT_PARTICLE) {}
	float m_lifeTime = 0.0f;
};

struct EnnemyComponent : public Component
{
	EnnemyComponent() : Component(Ennemy_ID, COMPONENT_ENNEMY) {}
};

struct PlayerComponent : public Component
{
	PlayerComponent() : Component(Player_ID, COMPONENT_PLAYER) {}
};

struct LifeTimeComponent : public Component
{
	LifeTimeComponent() : Component(LifeTime_ID, COMPONENT_LIFETIME) {}
	float m_lifeTime = 0.0f;
};

struct SceneObjectComponent : public Component
{
	SceneObjectComponent() : Component(SceneObject_ID, COMPONENT_SCENEOBJECT) {}
	float speedRotX = 0.f;
	float speedRotY = 0.f;
	float speedRotZ = 0.f;
};

struct TransformComponent : public Component
{
	TransformComponent() : Component(Transform_ID, COMPONENT_TRANSFORM) {}
	Transform m_transform;	
};

struct VelocityComponent : public Component
{
	VelocityComponent() : Component(Velocity_ID, COMPONENT_VELOCITY) {}
	float vz = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
};

struct HealthComponent : public Component
{
	HealthComponent() : Component(Health_ID, COMPONENT_HEALTH) {}
	int maxHealth = 100;
	int currentHealth = maxHealth;
};

struct AttackComponent : public Component
{
	AttackComponent() : Component(Attack_ID, COMPONENT_ATTACK) {}
	
	int damage = 0; 
	float attackCooldown = 0.f; 
	float timeSinceLastAttack = 0.0f;
	bool attackRequested = false;

	float projectileSpeed = 0.f;
	float projectileSizeX = 0.f;
	float projectileSizeY = 0.f;
	float projectileSizeZ = 0.f;

	std::wstring projectileTexture = L"DefaultTexture";
};

enum class LightType {
	Point,
	Directional
};
struct LightComponent : public Component {
	LightComponent() : Component(Light_ID, COMPONENT_LIGHT) {}
	LightType type;
	// Pour la lumiere directionnelle
	DirectX::XMFLOAT3 Direction;
	// Pour la lumiere point (omnidirectionnelle)
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Color;
	// Parametres d'attenuation pour une lumiere point
	float ConstantAtt;
	float LinearAtt;
	float QuadraticAtt;
};

struct HighlightComponent : public Component {
	HighlightComponent() : Component(Highlight_ID, COMPONENT_HIGHLIGHT) {}

	bool isHighlighted;
	float intensity;
};
