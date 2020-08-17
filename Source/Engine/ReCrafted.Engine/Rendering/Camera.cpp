// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Camera.h"
#include "Core/Display.h"

Camera* Camera::m_mainCamera;

void Camera::UpdateRotation()
{
    auto rotationMatrix = Matrix::CreateRotation(m_rotation);

    m_forward = rotationMatrix.Forward();
    m_right = rotationMatrix.Right();
    m_up = rotationMatrix.Up();
}

void Camera::Update()
{
    // Update rotation
    UpdateRotation();

    // Update matrices
    m_lookAt = m_position + m_forward;

    // create view matrix
    m_view = Matrix::CreateLookAt(m_position, m_lookAt, m_up);

    // create projection matrix
    m_projection = Matrix::CreatePerspective(Math::DegreeToRadian * m_fov, Display::GetAspectRatio(), m_nearPlane, m_farPlane);

    // create view projection matrix
    m_viewProjection = m_view * m_projection;

    // Update camera frustum for culling
    m_frustum.SetPlanes(m_viewProjection);
}
