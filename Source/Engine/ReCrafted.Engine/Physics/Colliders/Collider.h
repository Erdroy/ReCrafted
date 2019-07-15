// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Object.h"
#include "Physics/PhysX.h"
#include "Physics/PhysicsMaterial.h"

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     Collider base class.
/// </summary>
API_CLASS(public, customNamespace = "ReCrafted.API.Physics")
class Collider : public Object
{
    API_CLASS_BODY()
    friend class RigidBodyActor;

protected:
    PxShape* m_shape = nullptr;
    PhysicsMaterial* m_material = nullptr;
    Vector3 m_localPosition = Vector3::Zero;
    Quaternion m_localRotation = Quaternion::Identity;

public:
    virtual ~Collider() = default;

public:
    /// <summary>
    ///     Gets or sets the local position offset of this box collider.
    /// </summary>
    API_PROPERTY()
    void LocalPosition(const Vector3& position);

    /// <summary>
    ///     Gets or sets the local position offset of this box collider.
    /// </summary>
    API_PROPERTY()
    const Vector3& LocalPosition() const;

    /// <summary>
    ///     Gets or sets the local rotation of this box collider.
    /// </summary>
    API_PROPERTY()
    void LocalRotation(const Quaternion& rotation);

    /// <summary>
    ///     Gets or sets the local rotation of this box collider.
    /// </summary>
    API_PROPERTY()
    const Quaternion& LocalRotation() const;

    /// <summary>
    ///     Gets or sets the physics material of this collider.
    /// </summary>
    API_PROPERTY(byvalue)
    void Material(PhysicsMaterial* material);

    /// <summary>
    ///     Gets or sets the physics material of this collider.
    /// </summary>
    API_PROPERTY()
    PhysicsMaterial* Material() const;
};
