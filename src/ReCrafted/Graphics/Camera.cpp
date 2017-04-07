// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "../Core/GameCore.h"

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

void Camera::updateControls()
{
	// update movement
	Vector3 direction = {};

	if (Input::isKey(Key_W))
		direction += m_forward;

	if (Input::isKey(Key_S))
		direction -= m_forward;

	if (Input::isKey(Key_A))
		direction -= m_right;

	if (Input::isKey(Key_D))
		direction += m_right;

	if (Input::isKey(Key_Q))
		direction -= m_up;

	if (Input::isKey(Key_E))
		direction += m_up;

	direction.normalize();

	direction *= float(Time::deltaTime());

	// shift to speed up
	auto mul = Input::isKey(Key_Shift) ? 50.0f : 15.0f;

	// ctrl to slow down
	mul = Input::isKey(Key_Control) ? 1.0f : mul;

	direction *= mul;

	if(direction.length() > 0.0f)
		m_position += direction;

	// update look
	auto center = Vector2(
		round(GameCore::getWindowWidth() / 2.0f), 
		round(GameCore::getWindowHeight() / 2.0f)
	);

	auto cursorPos = Input::getCursorPos();
	auto rawDelta = center - cursorPos;

	// update filtering buffer
	m_cursorDeltaBuffer[m_cursorDeltaBufferPosition] = rawDelta;
	m_cursorDeltaBufferPosition++;

	// reset if needed(out of bounds)
	if (m_cursorDeltaBufferPosition >= FILTERING_BUFFER_SIZE)
		m_cursorDeltaBufferPosition = 0u;

	// calculate delta
	Vector2 delta = {};
	for (auto i = 0u; i < FILTERING_BUFFER_SIZE; i++)
	{
		delta += m_cursorDeltaBuffer[i];
	}

	delta.x = delta.x / FILTERING_BUFFER_SIZE;
	delta.y = delta.y / FILTERING_BUFFER_SIZE;

	// accelerate
	auto accelDelta = delta + m_lastDelta;

	// apply camera rotation
	m_rotation += Vector3(accelDelta.x / 16.0f, accelDelta.y / 16.0f, 0.0f);
	m_rotation.y = Math::clamp(m_rotation.y, -89.9f, 89.9f);

	// update camera rotation
	updateRotation();

	// save needed data
	m_lastDelta = delta;
}

void Camera::updatePerspective()
{
	// create projection matrix
	Matrix::createPerspectiveFovLH(Math::degreeToRadian(m_fov), GameCore::getAspectRatio(), m_nearPlane, m_farPlane, &m_projection);
}

void Camera::update()
{
	// update movement if this camera is 'free'.
	if (m_freeMovement && Input::isKey(Key_Mouse1)) 
	{
		updateControls(); // just do it!
		
		// lock position
		Input::setCursorPos(
			static_cast<int>(round(GameCore::getWindowWidth() / 2.0f)), 
			static_cast<int>(round(GameCore::getWindowHeight() / 2.0f))
		);
	}

	// update matrices
	m_lookAt = m_position + m_forward;

	// create view matrix
	Matrix::createViewLH(m_position, m_lookAt, m_upLock, &m_view);

	// update frustum
	// TODO: camera frustum for culling
}
