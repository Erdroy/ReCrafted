// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Physics/Colliders/Collider.h"

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     SphereCollider class.
/// </summary>
API_CLASS(public, customNamespace = "ReCrafted.API.Physics")
class SphereCollider final : public Collider
{
    API_CLASS_BODY()
    DELETE_COPY_MOVE(SphereCollider)

private:
    float m_radius = 1.0f;

public:
    SphereCollider();
    ~SphereCollider();

public:
    /// <summary>
    ///     Gets or sets the radius of this sphere collider.
    /// </summary>
    API_PROPERTY()
    void Radius(float radius);

    /// <summary>
    ///     Gets or sets the radius of this sphere collider.
    /// </summary>
    API_PROPERTY()
    float Radius() const;
};
