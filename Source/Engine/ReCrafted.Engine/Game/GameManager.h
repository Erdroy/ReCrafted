// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Scripting/Method.h"

/// <summary>
///     GameManager class. Provides basic game management services. 
///     Updates managed game class instance (derived from ReCrafted.API.Core.GameBase).
/// </summary>
class GameManager
{
    DELETE_OPERATOR_COPY_MOVE(GameManager)
    DELETE_CTOR_COPY(GameManager)
    friend class Application;

private:
    Object* m_game = nullptr;

    Method m_gameShutdown = Method{ nullptr };
    Method m_gameUpdate = Method{ nullptr };
    Method m_gameFixedUpdate = Method{ nullptr };

private:
    void tmpOnMaterialLoaded(Asset* materialAsset);

public:
    GameManager();
    ~GameManager();

protected:
    void Update() const;
    void FixedUpdate() const;
};
