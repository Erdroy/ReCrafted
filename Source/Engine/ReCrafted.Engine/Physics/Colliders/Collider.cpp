// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Collider.h"
#include "Physics/PhysicsMaterial.h"

Collider::~Collider()
{
    PX_RELEASE(m_shape);
}

void Collider::LocalPosition(const Vector3& position)
{
    if (!m_shape) return;

    m_localPosition = position;

    const auto pxTransform = PxTransform(m_localPosition.x, m_localPosition.y, m_localPosition.z, ToPxQ(m_localRotation));
    ASSERT(pxTransform.isValid());
    m_shape->setLocalPose(pxTransform);
}

const Vector3& Collider::LocalPosition() const
{
    return m_localPosition;
}

void Collider::LocalRotation(const Quaternion& rotation)
{
    if (!m_shape) return;

    m_localRotation = rotation;

    const auto pxTransform = PxTransform(m_localPosition.x, m_localPosition.y, m_localPosition.z, ToPxQ(m_localRotation));
    ASSERT(pxTransform.isValid());
    m_shape->setLocalPose(pxTransform);
}

const Quaternion& Collider::LocalRotation() const
{
    return m_localRotation;
}

void Collider::Material(PhysicsMaterial* material)
{
    m_material = material;
    if (!m_shape) return;

    PxMaterial* materials[] = { material->GetPxMaterial() };
    m_shape->setMaterials(materials, 1u);
}

PhysicsMaterial* Collider::Material() const
{
    return m_material;
}
