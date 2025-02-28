#pragma once

#include "Scene.h" 

class GameScene : public Scene
{
public:

	void CreateDefaultBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ, int health);

	void CreateWallBlock(float sizeX, float sizeY, float sizeZ, float posX, float posY, float posZ);

	void OnInitialize() override;

	void OnUpdate() override;

	void OnClose() override;

protected:

	int compteur;
	int compteur2;

	Entity* playerEntity = nullptr;
	Entity* iceBlockEntity = nullptr;
};

