#pragma once

#include "Transform.h"

#include "MeshFactory.h"

#include "Camera.h"

#include "d3dx12.h"

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

	TOTALCOMPONENT
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
};

struct GeometryMesh
{
	// Vertex Buffer
	ComPtr<ID3D12Resource> m_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

	// Index Buffer
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

	// Map
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
	//Camera m_camera;

	DirectX::XMMATRIX cameraView;
};

struct MeshComponent : public Component
{
	MeshComponent() : Component(Mesh_ID, COMPONENT_MESH) {}
	Mesh* m_cubeMesh;
	std::wstring textureID = L""; // identifiant de texture
};

struct ColliderComponent : public Component
{
	ColliderComponent() : Component(Collider_ID, COMPONENT_COLLIDER) {}
	bool m_isColliding = false;
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
	int damage = 10;
	float attackCooldown = 0.1;
	float timeSinceLastAttack = 0.0f; 
	bool attackRequested = false;     
	Entity* targetEntity = nullptr;   
};


//class PositionComponent : public Component
//{
//};
