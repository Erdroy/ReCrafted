// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

// includes
#include "Core/Defines.h"
#include "Core/Math/Math.h"
#include "Core/Math/BoundingFrustum.h"
#include "Scripting/Object.h"

class Entity;

/// <summary>
/// Camera class.
/// </summary>
class Camera : public Object
{
    friend class Graphics;

private:
SCRIPTING_API_IMPL()

private:
    static Camera* m_mainCamera;

private:
    Vector3 m_upLock = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 m_lookAt = {};

    // camera matrices
    Matrix m_view = {};
    Matrix m_projection = {};

    BoundingFrustum m_frustum = {};

private:
    void updateRotation();
    void update();

public:
    /// <summary>
    /// Default Camera constructor.
    /// </summary>
    Camera()
    {
        m_forward = Vector3::forward();
        m_right = Vector3::right();
        m_up = Vector3::up();

        // initialize
        update();

        // set as main camera if there is no any other
        if (m_mainCamera == nullptr)
            m_mainCamera = this;
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
    /// Gets the main camera.
    /// </summary>
    /// <returns></returns>
    FORCEINLINE static Camera* getMainCamera()
    {
        return m_mainCamera;
    }

public:
PROPERTY(bool, freeMovement) = false;
PROPERTY(float, fov) = 75.0f;
PROPERTY(float, farPlane) = 1000.0f;
PROPERTY(float, nearPlane) = 0.02f;
PROPERTY_REF(Vector3, position) = {};
PROPERTY_REF(Vector3, rotation) = {};
PROPERTY_REF(Vector3, forward) = {};
PROPERTY_REF(Vector3, up) = {};
PROPERTY_REF(Vector3, right) = {};
PROPERTY_REF(Matrix, viewProjection) = {};
};

#endif // CAMERA_H
