// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Object.h"
#include "Physics/PhysX.h"

/// <summary>
///     PhysicsMaterial class.
/// </summary>
API_CLASS(public, sealed)
class PhysicsMaterial final : public Object
{
    API_CLASS_BODY()
    friend class PhysicsManager;

protected:
    static PhysicsMaterial* m_defaultMaterial;
    PxMaterial* m_material;

public:
    PhysicsMaterial();
    ~PhysicsMaterial();

public:
    PxMaterial* GetPxMaterial() const
    {
        return m_material;
    }

public:
    /// <summary>
    ///     Gets or sets the static friction material value. (default: 0.7)
    /// </summary>
    API_PROPERTY()
    void StaticFriction(float friction) const;

    /// <summary>
    ///     Gets or sets the static friction material value. (default: 0.7)
    /// </summary>
    API_PROPERTY()
    float StaticFriction() const;

    /// <summary>
    ///     Gets or sets the dynamic friction material value. (default: 0.7)
    /// </summary>
    API_PROPERTY()
    void DynamicFriction(float friction) const;

    /// <summary>
    ///     Gets or sets the dynamic friction material value. (default: 0.7)
    /// </summary>
    API_PROPERTY()
    float DynamicFriction() const;

    /// <summary>
    ///     Gets or sets the restitution material value. (default: 0.25)
    /// </summary>
    API_PROPERTY()
    void Restitution(float restitution) const;

    /// <summary>
    ///     Gets or sets the restitution material value. (default: 0.25)
    /// </summary>
    API_PROPERTY()
    float Restitution() const;

public:
    /// <summary>
    ///     Gets the default physics material.
    /// </summary>
    API_PROPERTY()
    static PhysicsMaterial* Default();
};
