// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Camera.h"
#include "../Core/GameCore.h"

// declare m_mainCamera static variable
Camera* Camera::m_mainCamera;

void Camera::updateControls()
{
}

void Camera::updatePerspective()
{
	// create projection matrix
	matrix_perspective_xfov_LH(m_projection, m_xfov, GameCore::getAspectRatio(), m_nearPlane, m_farPlane, z_clip_zero);
}

void Camera::update()
{
	// update movement if this camera is 'free'.
	if (m_freeMovement)
		updateControls(); // just do it!

	// update matrices

	// create view matrix
	matrix_look_at_LH(m_view, m_position, m_lookAt, m_up);

	// update frustum
	// TODO: camera frustum for culling
}
