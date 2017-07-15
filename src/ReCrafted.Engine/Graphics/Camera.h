// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

// includes
#include "Utils/Types.h"
#include "Utils/Defines.h"
#include "Core/Math/Math.h"
#include "Core/Math/BoundingFrustum.h"

#define FILTERING_BUFFER_SIZE 3

class Entity;

/// <summary>
/// Camera class.
/// </summary>
class Camera
{
	friend class Rendering;
	friend class GameMain;

private:
	static Camera* m_mainCamera;

private:
	bool m_freeMovement = false;
	bool m_cursorLocked = true;

	Vector3 m_upLock = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 m_lookAt = {};
	Vector3 m_rotation = {};

	// camera matrices
	Matrix m_view = {};
	Matrix m_projection = {};

	Vector2 m_lastDelta = {};
	Vector2 m_cursorDeltaBuffer[FILTERING_BUFFER_SIZE] = {};
	uint m_cursorDeltaBufferPosition = 0u;

	BoundingFrustum m_frustum = {};

private:
	void updateRotation();
	void updateControls();
	void updatePerspective();
	void update();

private:
	Camera()
	{
		// clear filtering buffer
		for(auto i = 0u; i < FILTERING_BUFFER_SIZE; i ++)
		{
			m_cursorDeltaBuffer[i] = {};
		}
	}

public:
	/// <summary>
	/// Sets this camera as current.
	/// </summary>
	FORCEINLINE void setAsCurrent()
	{
		m_mainCamera = this;
	}

	/// <summary>
	/// Sets the camera 'look-at'.
	/// </summary>
	/// <param name="lookAt"></param>
	FORCEINLINE void setLookAt(Vector3 lookAt)
	{
		m_lookAt = lookAt;
	}

	/// <summary>
	/// Gets the camera bounding frustum.
	/// </summary>
	/// <returns>The bounding frustum.</returns>
	FORCEINLINE BoundingFrustum& getBoundingFrustum()
	{
		return m_frustum;
	}

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

	/// <summary>
	/// Gets the main camera.
	/// </summary>
	/// <returns></returns>
	FORCEINLINE static Camera* getMainCamera()
	{
		return m_mainCamera;
	}

public:
	PROPERTY(float, fov) = 75.0f;
	PROPERTY(float, farPlane) = 1000.0f;
	PROPERTY(float, nearPlane) = 0.02f;
	PROPERTY(Vector3, position) = {};
	PROPERTY(Vector3, forward) = {};
	PROPERTY(Vector3, up) = {};
	PROPERTY(Vector3, right) = {};
};

#endif // CAMERA_H