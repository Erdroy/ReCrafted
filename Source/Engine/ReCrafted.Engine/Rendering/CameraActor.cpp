// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "CameraActor.h"
#include "Camera.h"

CameraActor::~CameraActor()
{
    ASSERT(m_camera);
    DestroyNow(m_camera);
}

void CameraActor::OnAwake()
{
    // Create camera
    m_camera = Camera::Create();
    m_camera->SetAsCurrent();
    ASSERT(m_camera);
}

void CameraActor::OnLateUpdate()
{
    ASSERT(m_camera);
    m_camera->Position(Position());
    m_camera->Rotation(Rotation());
}

void CameraActor::OnEnable()
{
    ASSERT(m_camera);
    m_camera->SetAsCurrent();
}

void CameraActor::OnDisable()
{
}
