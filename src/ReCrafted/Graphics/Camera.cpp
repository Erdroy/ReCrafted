// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "../Core/GameCore.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

void Camera::updateControls()
{
	vector3f direction = {};

	if (Input::isKey(Key_W))
		direction += vector3f(0.0f, 0.0f, 1.0f);

	if (Input::isKey(Key_S))
		direction += vector3f(0.0f, 0.0f, -1.0f);

	if (Input::isKey(Key_A))
		direction += vector3f(-1.0f, 0.0f, 0.0f);

	if (Input::isKey(Key_D))
		direction += vector3f(1.0f, 0.0f, 0.0f);

	if (Input::isKey(Key_Q))
		direction += vector3f(0.0f, -1.0f, 0.0f);

	if (Input::isKey(Key_E))
		direction += vector3f(0.0f, 1.0f, 0.0f);

	direction = direction.normalize();

	direction *= Time::deltaTime();

	if(direction.length() > 0.0001f)
		m_position = m_position + direction;
}

void Camera::updatePerspective()
{
	// create projection matrix
	matrix_perspective_xfov_LH(m_projection, m_xfov, GameCore::getAspectRatio(), m_nearPlane, m_farPlane, z_clip_zero);
	m_projection = m_projection.transpose();
}

void Camera::update()
{
	// update movement if this camera is 'free'.
	if (m_freeMovement)
		updateControls(); // just do it!

	// update matrices
	m_lookAt = m_position + m_forward;

	// create view matrix
	matrix_look_at_LH(m_view, m_position, m_lookAt, m_up);
	m_view = m_view.transpose();

	// update frustum
	// TODO: camera frustum for culling
}
