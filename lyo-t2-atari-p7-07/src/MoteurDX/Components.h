#pragma once

//#include <DirectXMath.h>
//#include "Transform.h"
#include "MeshFactory.h"


enum ComponentType {
	COMPONENT_NONE = 0,
	COMPONENT_CAMERA = 1 << 0,
	COMPONENT_MESH = 1 << 1,
	COMPONENT_TRANSFORM = 1 << 2,
	COMPONENT_VELOCITY = 1 << 3,
	COMPONENT_HEALTH = 1 << 4,

	COMPONENT_HEAL = 1 << 5,		// Exemple

	TOTALCOMPONENT
};

enum ComponentIndex
{
	Camera_index,
	Mesh_index,
	Transform_index,
	Velocity_index,
	Health_index,

	Heal_index		// Exemple
};

enum ComponentID
{
	Camera_ID,
	Mesh_ID,
	Transform_ID,
	Velocity_ID,
	Health_ID,

	Heal_ID		// Exemple
};

struct Component
{
	//int ID = -1;
	//ComponentMask mask = 0;
};

struct CameraComponent : public Component
{
	int ID = Camera_ID;
	ComponentMask mask = COMPONENT_CAMERA; 
};

struct MeshComponent : public Component
{
	int ID = Mesh_ID;
	ComponentMask mask = COMPONENT_MESH;
	CubeMesh m_cubeMesh;
};

struct TransformComponent : public Component
{
	int ID = Transform_ID;
	ComponentMask mask = COMPONENT_TRANSFORM;
	Transform m_transform;	
};


struct VelocityComponent : public Component
{
	int ID = Velocity_ID;
	ComponentMask mask = COMPONENT_VELOCITY;
	float vz = 0.0f;
	float vx = 0.0f;
	float vy = 0.0f;
};


//class PositionComponent : public Component
//{
	// /!\/!\/!\/!\/!\/!\/!\/!\/!\/!\  ---   Pourrait etre utilise pour optimiser l'utilisation : pas besoin du Transform entier tout le temps je pense ?   ---   /!\/!\/!\/!\/!\/!\/!\/!\/!\/!\
};
