// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>
#include "Scripting/Object.h"

API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Camera class.
/// </summary>
API_CLASS(public, sealed, partial)
class Camera : public Object
{
    friend class RenderingManager;
    API_CLASS_BODY()

private:
    static Camera* m_mainCamera;

private:
    Vector3 m_upLock = Vector3(0.0f, 1.0f, 0.0f);
    Vector3 m_lookAt = {};

    // camera matrices
    Matrix m_view = {};
    Matrix m_projection = {};
    Matrix m_viewProjection = {};

    BoundingFrustum m_frustum = {};

    Vector3 m_forward{};
    Vector3 m_right{};
    Vector3 m_up{};

    float m_fov = 75.0f;
    float m_nearPlane = 0.1f;
    float m_farPlane = 10000.0f;

    Vector3 m_position{};
    Quaternion m_rotation{};

public:
    /// <summary>
    ///     Default Camera constructor.
    /// </summary>
    Camera()
    {
        m_forward = Vector3::Forward;
        m_right = Vector3::Right;
        m_up = Vector3::Up;

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
    ///     Sets this camera as current.
    /// </summary>
    API_FUNCTION()
    void SetAsCurrent()
    {
        m_mainCamera = this;
    }

    /// <summary>
    ///     Sets the camera 'look-at'.
    /// </summary>
    /// <param name="lookAt">The look at vector.</param>
    API_FUNCTION()
    void SetLookAt(const Vector3& lookAt)
    {
        m_lookAt = lookAt;
    }

    /// <summary>
    ///     Gets the camera bounding frustum.
    /// </summary>
    /// <returns>The bounding frustum.</returns>
    API_PROPERTY(noprefix)
    const BoundingFrustum& GetBoundingFrustum() const
    {
        return m_frustum;
    }

    /// <summary>
    ///     Gets projection matrix of this camera.
    /// </summary>
    API_PROPERTY(noprefix)
    const Matrix& GetProjection() const
    {
        return m_projection;
    }

    /// <summary>
    ///     Gets view matrix of this camera.
    /// </summary>
    API_PROPERTY(noprefix)
    const Matrix& GetView() const
    {
        return m_view;
    }

    /// <summary>
    ///     Gets view projection matrix of this camera.
    /// </summary>
    API_PROPERTY(noprefix)
    const Matrix& GetViewProjection() const
    {
        return m_viewProjection;
    }

    /// <summary>
    ///     Gets or sets the camera's position.
    /// </summary>
    API_PROPERTY()
    void Position(const Vector3& position)
    {
        m_position = position;
    }

    /// <summary>
    ///     Gets or sets the camera's position.
    /// </summary>
    API_PROPERTY()
    const Vector3& Position() const
    {
        return m_position;
    }
    
    /// <summary>
    ///     Gets or sets the camera's rotation.
    /// </summary>
    API_PROPERTY()
    void Rotation(const Quaternion& rotation)
    {
        m_rotation = rotation;
    }

    /// <summary>
    ///     Gets or sets the camera's rotation.
    /// </summary>
    API_PROPERTY()
    const Quaternion& Rotation() const
    {
        return m_rotation;
    }
    
    /// <summary>
    ///     Gets or sets the camera's fov.
    /// </summary>
    API_PROPERTY()
    void Fov(const float fov)
    {
        m_fov = fov;
    }

    /// <summary>
    ///     Gets or sets the camera's fov.
    /// </summary>
    API_PROPERTY()
    float Fov() const
    {
        return m_fov;
    }
    
    /// <summary>
    ///     Gets or sets the camera's near view plane distance.
    /// </summary>
    API_PROPERTY()
    void NearPlane(const float nearPlane)
    {
        m_nearPlane = nearPlane;
    }

    /// <summary>
    ///     Gets or sets the camera's near view plane distance.
    /// </summary>
    API_PROPERTY()
    float NearPlane() const
    {
        return m_nearPlane;
    }
    
    /// <summary>
    ///     Gets or sets the camera's far view plane distance.
    /// </summary>
    API_PROPERTY()
    void FarPlane(const float farPlane)
    {
        m_farPlane = farPlane;
    }

    /// <summary>
    ///     Gets or sets the camera's far view plane distance.
    /// </summary>
    API_PROPERTY()
    float FarPlane() const
    {
        return m_farPlane;
    }

    /// <summary>
    ///     Gets the camera's forward vector.
    /// </summary>
    API_PROPERTY()
    const Vector3& Forward() const
    {
        return m_forward;
    }

    /// <summary>
    ///     Gets the camera's right vector.
    /// </summary>
    API_PROPERTY()
    const Vector3& Right() const
    {
        return m_right;
    }

    /// <summary>
    ///     Gets the camera's up vector.
    /// </summary>
    API_PROPERTY()
    const Vector3& Up() const
    {
        return m_up;
    }

public:
    /// <summary>
    ///     Gets the main camera.
    /// </summary>
    /// <returns></returns>
    API_PROPERTY(noprefix)
    static Camera* GetMainCamera()
    {
        return m_mainCamera;
    }

    /// <summary>
    ///     Creates new camera instance.
    /// </summary>
    /// <returns>The new camera instance.</returns>
    API_FUNCTION()
    static Camera* Create()
    {
        return New<Camera>();
    }
};
