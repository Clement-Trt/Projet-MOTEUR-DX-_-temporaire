#pragma once

//#include <DirectXMath.h>
#include "Transform.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <wrl.h>
#include "d3dx12.h"
#include <d3dcompiler.h>

using namespace Microsoft::WRL;

enum ComponentType {
	COMPONENT_NONE = 0,
	COMPONENT_TRANSFORM = 1 << 0,
	COMPONENT_MESH = 1 << 1,
	COMPONENT_VELOCITY = 1 << 2,
	COMPONENT_HEALTH = 1 << 3,

	COMPONENT_HEAL = 1 << 4,		// Exemple

	TOTALCOMPONENT
};

enum ComponentIndex
{
	Transform_index,
	Mesh_index,
	Velocity_index,
	Health_index,

	Heal_index		// Exemple
};

struct CubeMesh
{
	// Buffer de constantes (matrices, couleurs, etc.)
	ComPtr<ID3D12Resource> m_constantBuffer;

	// Map
	void* m_mappedData = nullptr;

	// Nombre d'indices pour le rendu
	UINT m_meshIndex;

	// InitMap
	void InitConstantBuffer()
	{
		CD3DX12_RANGE readRange(0, 0);
		m_constantBuffer->Map(0, &readRange, &m_mappedData);
	}
};

class Component
{

};

struct MeshComponent : public Component
{
	CubeMesh* m_cubeMesh;
};


//class PositionComponent : public Component
//{
	// /!\/!\/!\/!\/!\/!\/!\/!\/!\/!\  ---   Pourrait etre utilise pour optimiser l'utilisation : pas besoin du Transform entier tout le temps je pense ?   ---   /!\/!\/!\/!\/!\/!\/!\/!\/!\/!\
};

struct CameraComponent : public Component
{

};

struct TransformComponent : public Component
{
	Transform m_transform;	
};

struct VelocityComponent : public Component
{
	float vz = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
};

