// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Physics/Colliders/Collider.h"

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     BoxCollider class.
/// </summary>
API_CLASS(public, customNamespace="ReCrafted.API.Physics")
class BoxCollider final : public Collider
{
    API_CLASS_BODY()
    DELETE_COPY_MOVE(BoxCollider)

private:
    Vector3 m_size = Vector3::One;

public:
    BoxCollider();
    ~BoxCollider();

public:
    /// <summary>
    ///     Gets or sets the size of this box collider.
    /// </summary>
    API_PROPERTY()
    void Size(const Vector3& size);

    /// <summary>
    ///     Gets or sets the size of this box collider.
    /// </summary>
    API_PROPERTY()
    const Vector3& Size() const;
};
