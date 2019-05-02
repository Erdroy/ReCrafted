// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Common/Lock.h"
#include "Core/SubSystems/SubSystem.h"

#include <sparsepp/spp.h>

using ObjectId_t = uint32_t;
using GCHandle_t = uint32_t;

class ObjectManager final : public SubSystem<ObjectManager>
{
private:
    std::atomic<ObjectId_t> m_lastObjectId = 0;
    std::atomic<GCHandle_t> m_objectCount = 0;
    spp::sparse_hash_map<GCHandle_t, Object*> m_objectMap;
    Lock m_objectMapLock;

private:
    void RegisterObject(Object* object);

protected:
    void Initialize() override;
    void Shutdown() override;

public:
    /// <summary>
    ///     Initializes managed instance of given object.
    /// </summary>
    /// <param name="object">The object instance.</param>
    /// <param name="nameSpace">The namespace.</param>
    /// <param name="name">The class name.</param>
    /// <param name="assembly">The assembly.</param>
    /// <param name="domain">The domain.</param>
    void InitializeInstance(Object* object, const char* nameSpace, const char* name, const RefPtr<Assembly>& assembly, const RefPtr<Domain>& domain);

    /// <summary>
    ///     Destroys given Object instance.
    /// </summary>
    static void Destroy(Object* objectInstance);
    
    /// <summary>
    ///     Destroys given Object instance now.
    /// </summary>
    static void DestroyNow(Object* objectInstance);
};
