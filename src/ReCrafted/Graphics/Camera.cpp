// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "../Core/GameCore.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

void Camera::updateRotation()
{
	auto yaw = Math::degreeToRadian(-m_rotation.Y);
	auto pitch = Math::degreeToRadian(m_rotation.X);

	// http://stackoverflow.com/questions/1568568/how-to-convert-euler-angles-to-directional-vector
	m_forward.X = Math::sinx(yaw);
	m_forward.Y = -(Math::sinx(pitch) * Math::cosx(yaw));
	m_forward.Z = -(Math::cosx(pitch) * Math::cosx(yaw));
}

void Camera::updateControls()
{
	// update movement
	Vector3 direction = {};

	if (Input::isKey(Key_W))
		direction += Vector3(0.0f, 0.0f, 1.0f);

	if (Input::isKey(Key_S))
		direction += Vector3(0.0f, 0.0f, -1.0f);

	if (Input::isKey(Key_A))
		direction += Vector3(-1.0f, 0.0f, 0.0f);

	if (Input::isKey(Key_D))
		direction += Vector3(1.0f, 0.0f, 0.0f);

	if (Input::isKey(Key_Q))
		direction += Vector3(0.0f, -1.0f, 0.0f);

	if (Input::isKey(Key_E))
		direction += Vector3(0.0f, 1.0f, 0.0f);

	direction.normalize();

	direction *= float(Time::deltaTime());

	if(direction.length() > 0.0f)
		m_position += direction;

	// update look
	auto center = Vector2(GameCore::getWindowWidth() / 2.0f, GameCore::getWindowHeight() / 2.0f);

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

	delta.X = delta.X / FILTERING_BUFFER_SIZE;
	delta.Y = delta.Y / FILTERING_BUFFER_SIZE;

	// accelerate
	auto accelDelta = delta + m_lastDelta;

	// apply camera rotation
	m_rotation += Vector3(accelDelta.X / 16.0f, -accelDelta.Y / 16.0f, 0.0f);
	m_rotation.Y = Math::clmap(m_rotation.Y, -89.9f, 89.9f);

	// update camera rotation
	updateRotation();

	// save needed data
	m_lastDelta = delta;
}

void Camera::updatePerspective()
{
	// create projection matrix
	Matrix::createPerspectiveFovLH(m_xfov, GameCore::getAspectRatio(), m_nearPlane, m_farPlane, &m_projection);
}

void Camera::update()
{
	// update movement if this camera is 'free'.
	if (m_freeMovement) 
	{
		updateControls(); // just do it!
		
		// lock position
		Input::setCursorPos(GameCore::getWindowWidth() / 2, GameCore::getWindowHeight() / 2);
	}

	// update matrices
	m_lookAt = m_position - m_forward;

	// create view matrix
	Matrix::createViewLH(m_position, m_lookAt, m_up, &m_view);

	// update frustum
	// TODO: camera frustum for culling
}
