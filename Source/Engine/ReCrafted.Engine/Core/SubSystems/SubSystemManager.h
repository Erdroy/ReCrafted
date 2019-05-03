// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/Singleton.h"
#include "Common/Lock.h"
#include "Common/List.h"

#include "Core/SubSystems/SubSystemBase.h"

class SubSystemManager final : public Singleton<SubSystemManager>
{
    friend class Application;

    DELETE_COPY_MOVE(SubSystemManager)

private:
    Lock m_lockSubSystems;
    List<SubSystemBase*> m_subSystems;

public:
    SubSystemManager();
    ~SubSystemManager();

private:
    void Shutdown();
    void Release(SubSystemBase* subsystem);

    void Update();
    void FixedUpdate();
    void LateUpdate();
    void FrameDone();

private:
    /**
     * \brief Registers subsystem class instance.
     * \param subsystem The class instance.
     *
     * \note Subsystems are automatically released in reverse order
     * that they are registered at shutdown.
     * 
     * \remarks This method locks subsystems list, and CANNOT be invoked from UPDATE, SIMULATE or RENDER call!
     */
    void Register(SubSystemBase* subsystem);

    /**
     * \brief Unregisters subsystem and releases it's memory.
     * \param subsystem The class instance.
     *
     * \remarks This method locks subsystems list, and CANNOT be invoked from UPDATE, SIMULATE or RENDER call!
     */
    void Unregister(SubSystemBase* subsystem);

public:
    /**
     * \brief Registers subsystem class instance.
     *
     * \note Subsystem CANNOT be released/destroyed by hand.
     * Also it must be a Singleton<TType>!
     * SubSystemManager takes care of that!
     * Subsystems are automatically released in reverse order
     * that they are registered.
     * 
     * \remarks This method locks subsystems list, and CANNOT be invoked from UPDATE, SIMULATE or RENDER call!
     */
    template <class TSubSystem>
    static void Register()
    {
        GetInstance()->Register(TSubSystem::GetInstance());
    }

    /**
     * \brief Unregisters subsystem and releases it's memory.
     *
     * \remarks This method locks subsystems list, and CANNOT be invoked from UPDATE, SIMULATE or RENDER call!
     */
    template <class TSubSystem>
    static void Unregister()
    {
        GetInstance()->Unregister(TSubSystem::GetInstance());
    }
};
