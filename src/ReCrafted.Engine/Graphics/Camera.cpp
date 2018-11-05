// ReCrafted (c) 2016-2018 Always Too Late

#include "Camera.h"
#include "Common/Display.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

void Camera::UpdateRotation()
{
    var rotationMatrix = Matrix::CreateRotation(m_Rotation);

    m_Forward = rotationMatrix.Forward();
    m_Right = rotationMatrix.Right();
    m_Up = rotationMatrix.Up();
}

void Camera::Update()
{
    // Update rotation
    UpdateRotation();

    // Update matrices
    m_lookAt = m_Position + m_Forward;

    // create view matrix
    m_view = Matrix::CreateLookAt(m_Position, m_lookAt, m_Up);

    // create projection matrix
    m_projection = Matrix::CreatePerspective(Math::DegreeToRadian * m_Fov, Display::GetAspectRatio(), m_NearPlane, m_FarPlane);

    // create view projection matrix
    m_ViewProjection = m_view * m_projection;

    // Update camera frustum for culling
    m_frustum.SetPlanes(m_ViewProjection);
}
