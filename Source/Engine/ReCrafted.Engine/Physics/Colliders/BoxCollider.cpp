// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "BoxCollider.h"
#include "Physics/PhysX.h"
#include "Physics/PhysicsManager.h"
#include "Physics/PhysicsMaterial.h"

BoxCollider::BoxCollider()
{
    m_material = PhysicsMaterial::Default();
    m_shape = PhysicsManager::GetPhysics()->createShape(PxBoxGeometry(ToPxV3((Vector3::One * 0.5f))), *m_material->GetPxMaterial());

    PxMaterial* materials[] = { m_material->GetPxMaterial() };
    m_shape->setMaterials(materials, 1u);
}

BoxCollider::~BoxCollider()
{
    PX_RELEASE(m_shape);
}

void BoxCollider::Size(const Vector3& size)
{
    m_size = size;

    const auto halfSize = size * 0.5f;
    m_shape->setGeometry(PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z));
}

const Vector3& BoxCollider::Size() const
{
    return m_size;
}
