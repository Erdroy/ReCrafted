// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "SphereCollider.h"
#include "Physics/PhysicsManager.h"

SphereCollider::SphereCollider()
{
    m_material = PhysicsMaterial::Default();
    m_shape = PhysicsManager::GetPhysics()->createShape(PxSphereGeometry(1.0f), *m_material->GetPxMaterial());

    PxMaterial* materials[] = { m_material->GetPxMaterial() };
    m_shape->setMaterials(materials, 1u);
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::Radius(const float radius)
{
    m_radius = radius;
    m_shape->setGeometry(PxSphereGeometry(m_radius));
}

float SphereCollider::Radius() const
{
    return m_radius;
}
