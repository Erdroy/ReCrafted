// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

// includes
#include "ReCrafted.h"
#include "Core/Math/BoundingFrustum.h"
#include "Scripting/Object.h"

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

public:
    /// <summary>
    /// Default Camera constructor.
    /// </summary>
    Camera()
    {
        m_Forward = Vector3::Forward;
        m_Right = Vector3::Right;
        m_Up = Vector3::Up;

        // initialize
        Update();

        // set as main camera if there is no any other
        if (m_mainCamera == nullptr)
            m_mainCamera = this;
    }

private:
    void UpdateRotation();
    void Update();

public:
    /// <summary>
    /// Sets this camera as current.
    /// </summary>
    FORCEINLINE void SetAsCurrent()
    {
        m_mainCamera = this;
    }

    /// <summary>
    /// Sets the camera 'look-at'.
    /// </summary>
    /// <param name="lookAt"></param>
    FORCEINLINE void SetLookAt(Vector3 lookAt)
    {
        m_lookAt = lookAt;
    }

    /// <summary>
    /// Gets the camera bounding frustum.
    /// </summary>
    /// <returns>The bounding frustum.</returns>
    FORCEINLINE BoundingFrustum& GetBoundingFrustum()
    {
        return m_frustum;
    }

    /**
     * \brief Gets projection matrix of this camera.
     */
    const Matrix& GetProjection() const
    {
        return m_projection;
    }

    /**
    * \brief Gets view matrix of this camera.
    */
    const Matrix& GetView() const
    {
        return m_view;
    }

public:
    /// <summary>
    /// Gets the main camera.
    /// </summary>
    /// <returns></returns>
    FORCEINLINE static Camera* GetMainCamera()
    {
        return m_mainCamera;
    }

    static Camera* Create();

public:
PROPERTY(bool, FreeMovement) = false;
PROPERTY(float, Fov) = 75.0f;
PROPERTY(float, FarPlane) = 10000.0f;
PROPERTY(float, NearPlane) = 0.02f;
PROPERTY_REF(Vector3, Position) = {};
PROPERTY_REF(Quaternion, Rotation) = {};
PROPERTY_REF(Vector3, Forward) = {};
PROPERTY_REF(Vector3, Up) = {};
PROPERTY_REF(Vector3, Right) = {};
PROPERTY_REF(Matrix, ViewProjection) = {};
};

#endif // CAMERA_H
