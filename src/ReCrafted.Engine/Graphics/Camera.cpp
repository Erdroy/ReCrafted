// ReCrafted © 2016-2017 Always Too Late

#include "Camera.h"
#include "Core/GameMain.h"
#include "Common/Display.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

void Camera::updateRotation()
{
	auto yaw = Math::degreeToRadian(-m_rotation.y);
	auto pitch = Math::degreeToRadian(-m_rotation.x);
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
	m_forward = Vector3(
		(1.0f - (single4 + single5)) * axis.x + (single6 - single11) * axis.y + (single7 + single10) * axis.z,
		(single6 + single11) * axis.x + (1.0f - (single3 + single5)) * axis.y + (single8 - single9) * axis.z,
		(single7 - single10) * axis.x + (single8 + single9) * axis.y + (1.0f - (single3 + single4)) * axis.z
	);

	// calculate right
	axis = Vector3(1.0f, 0.0f, 0.0f);
	m_right = Vector3(
		(1.0f - (single4 + single5)) * axis.x + (single6 - single11) * axis.y + (single7 + single10) * axis.z,
		(single6 + single11) * axis.x + (1.0f - (single3 + single5)) * axis.y + (single8 - single9) * axis.z,
		(single7 - single10) * axis.x + (single8 + single9) * axis.y + (1.0f - (single3 + single4)) * axis.z
	);

	// calculate up
	m_up = Vector3::cross(m_forward, m_right);
}

void Camera::updatePerspective()
{
	// create projection matrix
	Matrix::createPerspectiveFovLH(Math::degreeToRadian(m_fov), Display::getAspectRatio(), m_nearPlane, m_farPlane, &m_projection);
}

void Camera::update()
{
	// update rotation
	updateRotation();

	// update matrices
	m_lookAt = m_position + m_forward;

	// create view matrix
	Matrix::createViewLH(m_position, m_lookAt, m_upLock, &m_view);

	// update camera frustum for culling
	m_frustum = BoundingFrustum::FromCamera(m_position, m_forward, m_up, Math::degreeToRadian(m_fov), m_nearPlane, m_farPlane, Display::getAspectRatio());
}
