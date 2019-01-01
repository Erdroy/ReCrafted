// ReCrafted (c) 2016-2019 Always Too Late

#include "CameraActor.h"
#include "Core/Logger.h"

void CameraActor::OnAwake()
{
    // Create camera
    m_camera = Camera::Create();
    m_camera->SetAsCurrent();

#if DEVELOPMENT || DEBUG
    Logger::Log("CameraActor with id {0} has been created", m_camera->GetObjectId());
#endif
}

void CameraActor::OnDestroy()
{
    ASSERT(m_camera);
    Object::Destroy(m_camera);

#if DEVELOPMENT || DEBUG
    //Logger::Log("CameraActor with id {0} has been destroyed", m_camera->GetObjectId());
#endif
}

void CameraActor::OnLateUpdate()
{
    var position = GetPosition();
    var rotation = GetRotation();

    m_camera->SetPosition(position);
    m_camera->SetRotation(rotation);
}

void CameraActor::OnEnable()
{
    ASSERT(m_camera);

    m_camera->SetAsCurrent();

#if DEVELOPMENT || DEBUG
    Logger::Log("CameraActor with id {0} has been enabled and set as current", m_camera->GetObjectId());
#endif
}

void CameraActor::OnDisable()
{
#if DEVELOPMENT || DEBUG
    Logger::Log("CameraActor with id {0} has been disabled", m_camera->GetObjectId());
#endif
}
