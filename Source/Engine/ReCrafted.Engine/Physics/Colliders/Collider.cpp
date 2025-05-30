// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Collider.h"
#include "Physics/PhysicsMaterial.h"

Collider::~Collider()
{
    PX_RELEASE(m_shape);
}

void Collider::LocalPosition(const Vector3& position)
{
    ASSERT(m_shape);

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
    ASSERT(m_shape);
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
    ASSERT(m_shape);
    m_material = material;

    PxMaterial* materials[] = { material->GetPxMaterial() };
    m_shape->setMaterials(materials, 1u);
}

PhysicsMaterial* Collider::Material() const
{
    return m_material;
}

bool Collider::IsValid() const
{
    return m_shape != nullptr;
}
