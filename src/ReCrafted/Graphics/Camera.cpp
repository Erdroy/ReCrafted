// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "../Core/GameCore.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

// https://github.com/Erdroy/ReCrafted/blob/9138a5067abb88ddd3fc5dd2059c1d72e2b67535/src/ReCrafted/Graphics/Camera.cs

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
		updateControls(); // just do it!

	// update matrices
	m_lookAt = m_position + m_forward;

	// create view matrix
	Matrix::createViewLH(m_position, m_lookAt, m_up, &m_view);

	// update frustum
	// TODO: camera frustum for culling
}
