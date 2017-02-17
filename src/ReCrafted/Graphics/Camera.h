// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

// includes
#include "../Utils/Types.h"
#include "../Utils/Defines.h"
#include "../Core/Math/Math.h"

#define FILTERING_BUFFER_SIZE 3

class Entity;

class Camera
{
	friend class Rendering;

private:
	static Camera* m_mainCamera;

private:
	bool m_freeMovement = false;
	float m_nearPlane = 0.02f;
	float m_farPlane = 500.0f;
	float m_xfov = 0.75f;
	Vector3 m_position = Vector3(0.0f, 0.0f, -5.0f);
	Vector3 m_forward = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 m_up = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 m_lookAt = {};
	Vector3 m_rotation = {};

	Matrix m_view = {};
	Matrix m_projection = {};

	Vector2 m_lastDelta = {};
	Vector2 m_cursorDeltaBuffer[FILTERING_BUFFER_SIZE] = {};
	uint m_cursorDeltaBufferPosition = 0u;

private:
	void updateRotation();
	void updateControls();
	void updatePerspective();
	void update();

public:
	Camera()
	{
		// clear filtering buffer
		for(auto i = 0u; i < FILTERING_BUFFER_SIZE; i ++)
		{
			m_cursorDeltaBuffer[i] = {};
		}
	}

	FORCEINLINE void setAsCurrent()
	{
		m_mainCamera = this;
	}

	FORCEINLINE void setPosition(Vector3 position)
	{
		m_position = position;
	}

	FORCEINLINE void setLookAt(Vector3 lookAt)
	{
		m_lookAt = lookAt;
	}

	FORCEINLINE void setClipPlanes(float nearPlane, float farPlane)
	{
		m_nearPlane = nearPlane;
		m_farPlane = farPlane;
	}

	FORCEINLINE void setHorizontalFov(float fov)
	{
		m_xfov = fov;
	}

	void fix(Ptr<Entity> entity, Vector3 offset);

public:
	/// <summary>
	/// Create new camera.
	/// </summary>
	/// <param name="freeMovement">Is the camera free?(used for spectating, editor etc.)</param>
	/// <param name="setAsCurrent">Set this camera as current after initialization?
	/// This camera will be used to render the scene.</param>
	/// <returns></returns>
	FORCEINLINE static Ptr<Camera> createCamera(bool freeMovement = false, bool setAsCurrent = false)
	{
		// create new camera instance
		Ptr<Camera> camera(new Camera);

		// set free movement state
		camera->m_freeMovement = freeMovement;

		// initialize
		camera->update();
		camera->updatePerspective();

		// set as main camera if there is no any other, 
		// or set if there is `setAsCurrent` set to true
		if (m_mainCamera == nullptr || setAsCurrent)
			m_mainCamera = camera.get();

		return camera;
	}

	FORCEINLINE static Camera* getMainCamera()
	{
		return m_mainCamera;
	}
};

#endif // CAMERA_H