#pragma once

class InitDirect3DApp;

class HealthSystem
{
public:
    void Initialize(InitDirect3DApp* gameManager);

    void Update(float deltaTime);

protected:

    InitDirect3DApp* m_gameManager;

};