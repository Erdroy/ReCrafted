// ReCrafted (c) 2016-2018 Always Too Late

#include "Camera.h"
#include "Common/Display.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

void Camera::UpdateRotation()
{
    auto yaw = Math::DegreeToRadian * -m_Rotation.y;
    auto pitch = Math::DegreeToRadian * -m_Rotation.x;
    auto roll = 0.0f;

    auto yawOver2 = yaw * 0.5;
    auto cosYawOver2 = float(cos(yawOver2));
    auto sinYawOver2 = float(sin(yawOver2));

    auto pitchOver2 = pitch * 0.5;
    auto cosPitchOver2 = float(cos(pitchOver2));
    auto sinPitchOver2 = float(sin(pitchOver2));

    auto rollOver2 = roll * 0.5;
    auto cosRollOver2 = float(cos(rollOver2));
    auto sinRollOver2 = float(sin(rollOver2));

    // quaternion - from euler
    auto W = cosYawOver2 * cosPitchOver2 * cosRollOver2 + sinYawOver2 * sinPitchOver2 * sinRollOver2;
    auto X = sinYawOver2 * cosPitchOver2 * cosRollOver2 + cosYawOver2 * sinPitchOver2 * sinRollOver2;
    auto Y = cosYawOver2 * sinPitchOver2 * cosRollOver2 - sinYawOver2 * cosPitchOver2 * sinRollOver2;
    auto Z = cosYawOver2 * cosPitchOver2 * sinRollOver2 - sinYawOver2 * sinPitchOver2 * cosRollOver2;

    // quaternion * axis
    auto single = X * 2.0f;
    auto single1 = Y * 2.0f;
    auto single2 = Z * 2.0f;
    auto single3 = X * single;
    auto single4 = Y * single1;
    auto single5 = Z * single2;
    auto single6 = X * single1;
    auto single7 = X * single2;
    auto single8 = Y * single2;
    auto single9 = W * single;
    auto single10 = W * single1;
    auto single11 = W * single2;

    // calculate forward
    auto axis = Vector3(0.0f, 0.0f, 1.0f);
    m_Forward = Vector3(
        (1.0f - (single4 + single5)) * axis.x + (single6 - single11) * axis.y + (single7 + single10) * axis.z,
        (single6 + single11) * axis.x + (1.0f - (single3 + single5)) * axis.y + (single8 - single9) * axis.z,
        (single7 - single10) * axis.x + (single8 + single9) * axis.y + (1.0f - (single3 + single4)) * axis.z
    );

    m_Forward.Normalize();

    // calculate right
    axis = Vector3(1.0f, 0.0f, 0.0f);
    m_Right = Vector3(
        (1.0f - (single4 + single5)) * axis.x + (single6 - single11) * axis.y + (single7 + single10) * axis.z,
        (single6 + single11) * axis.x + (1.0f - (single3 + single5)) * axis.y + (single8 - single9) * axis.z,
        (single7 - single10) * axis.x + (single8 + single9) * axis.y + (1.0f - (single3 + single4)) * axis.z
    );
    m_Right.Normalize();

    // calculate up
    m_Up = Vector3::Cross(m_Forward, m_Right);
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
