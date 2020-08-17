// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "GameManager.h"
#include "Scripting/Object.h"
#include "Scripting/ScriptingManager.h"
#include "Profiler/Profiler.h"

GameManager::GameManager()
{
    m_game = Object::New("ReCrafted.Game.Core", "Game", ScriptingManager::GetGameAssembly());

    const auto gameInitialize = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnInitialize");
    ASSERT(gameInitialize.IsValid());

    m_gameShutdown = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnShutdown");
    ASSERT(m_gameShutdown.IsValid());

    m_gameUpdate = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnUpdate");
    ASSERT(m_gameUpdate.IsValid());

    m_gameFixedUpdate = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnFixedUpdate");
    ASSERT(m_gameFixedUpdate.IsValid());

    gameInitialize.Invoke(m_game);
}

GameManager::~GameManager()
{
    ASSERT(m_game);
    ASSERT(m_gameShutdown.IsValid());

    m_gameShutdown.Invoke(m_game);

    // Destroy game
    Object::Destroy(m_game);
}

void GameManager::Update() const
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameUpdate.IsValid());

    m_gameUpdate.Invoke(m_game);
}

void GameManager::FixedUpdate() const
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameFixedUpdate.IsValid());

    m_gameFixedUpdate.Invoke(m_game);
}
