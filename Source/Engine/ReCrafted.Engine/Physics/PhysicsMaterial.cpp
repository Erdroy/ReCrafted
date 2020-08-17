// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "PhysicsMaterial.h"
#include "PhysicsManager.h"

PhysicsMaterial* PhysicsMaterial::m_defaultMaterial;

PhysicsMaterial::PhysicsMaterial()
{
    m_material = PhysicsManager::GetPhysics()->createMaterial(0.7f, 0.7f, 0.25f);
    ASSERT(m_material);
}

PhysicsMaterial::~PhysicsMaterial()
{
    PX_RELEASE(m_material);
}

void PhysicsMaterial::StaticFriction(const float friction) const
{
    ASSERT(m_material);
    m_material->setStaticFriction(friction);
}

float PhysicsMaterial::StaticFriction() const
{
    ASSERT(m_material);
    return m_material->getStaticFriction();
}

void PhysicsMaterial::DynamicFriction(const float friction) const
{
    ASSERT(m_material);
    m_material->setDynamicFriction(friction);
}

float PhysicsMaterial::DynamicFriction() const
{
    ASSERT(m_material);
    return m_material->getDynamicFriction();
}

void PhysicsMaterial::Restitution(const float restitution) const
{
    ASSERT(m_material);
    m_material->setRestitution(restitution);
}

float PhysicsMaterial::Restitution() const
{
    ASSERT(m_material);
    return m_material->getRestitution();
}

PhysicsMaterial* PhysicsMaterial::Default()
{
    return m_defaultMaterial;
}
