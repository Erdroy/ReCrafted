// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENGINECOMPONENTMANAGER_H
#define ENGINECOMPONENTMANAGER_H

// includes
#include "Singleton.h"
#include "EngineComponent.h"
#include "Containers/Array.h"
#include "Lock.h"

/**
 * \brief EngineComponentManager class. 
 * Implements engine component utilization logic, components are initialized, invoked 
 * and destroyed here.
 */
class EngineComponentManager : public Singleton<EngineComponentManager>
{
    friend class EngineMain;

private:
    Lock m_componentsLock = {};
    Array<EngineComponentBase*> m_components = {};

private:
    void onLoad();
    void onDispose() override;

private:
    void releaseComponent(EngineComponentBase* component);

public:
    /**
     * \brief Registers engine component class instance.
     * \param component The class instance.
     * 
     * \note Component CANNOT be released/destroyed by hand. 
     * EngineComponentManager takes care of that!
     * Components are automaticaly released in reverse order 
     * that they are registered.
     */
    void registerComponent(EngineComponentBase* component);

    /**
     * \brief Unregisters engine component and releases it's memory.
     * \param component The class instance.
     * 
     * \note This method locks component list, and CANNOT be invoked from UPDATE, SIMULATE or RENDER call!
     */
    void unregisterComponent(EngineComponentBase* component);
};

#endif // ENGINECOMPONENTMANAGER_H
