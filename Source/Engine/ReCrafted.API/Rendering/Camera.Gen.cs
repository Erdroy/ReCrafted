// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Camera class.
    /// </summary>
    public sealed partial class Camera : Object
    {
        
        /// <summary>
        ///     Sets this camera as current.
        /// </summary>
        public void SetAsCurrent()
        {
            InternalSetAsCurrent(NativePtr);
        }
        
        /// <summary>
        ///     Sets the camera 'look-at'.
        /// </summary>
        /// <param name="lookAt">The look at vector.</param>
        public void SetLookAt(Vector3 lookAt)
        {
            InternalSetLookAt(NativePtr, lookAt);
        }
        
        /// <summary>
        ///     Creates new camera instance.
        /// </summary>
        /// <returns>The new camera instance.</returns>
        public static Camera Create()
        {
            return InternalCreate();
        }

        /// <summary>
        ///     Gets the camera bounding frustum.
        /// </summary>
        /// <returns>The bounding frustum.</returns>
        public BoundingFrustum BoundingFrustum
        {
            get
            {
                Get_InternalBoundingFrustum(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets projection matrix of this camera.
        /// </summary>
        public Matrix Projection
        {
            get
            {
                Get_InternalProjection(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets view matrix of this camera.
        /// </summary>
        public Matrix View
        {
            get
            {
                Get_InternalView(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets view projection matrix of this camera.
        /// </summary>
        public Matrix ViewProjection
        {
            get
            {
                Get_InternalViewProjection(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets or sets the camera's position.
        /// </summary>
        public Vector3 Position
        {
            get
            {
                Get_InternalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalPosition(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the camera's rotation.
        /// </summary>
        public Quaternion Rotation
        {
            get
            {
                Get_InternalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRotation(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the camera's fov.
        /// </summary>
        public float Fov
        {
            get
            {
                Get_InternalFov(NativePtr, out var data);
                return data;
            }
            set => Set_InternalFov(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the camera's near view plane distance.
        /// </summary>
        public float NearPlane
        {
            get
            {
                Get_InternalNearPlane(NativePtr, out var data);
                return data;
            }
            set => Set_InternalNearPlane(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the camera's far view plane distance.
        /// </summary>
        public float FarPlane
        {
            get
            {
                Get_InternalFarPlane(NativePtr, out var data);
                return data;
            }
            set => Set_InternalFarPlane(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets the camera's forward vector.
        /// </summary>
        public Vector3 Forward
        {
            get
            {
                Get_InternalForward(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the camera's right vector.
        /// </summary>
        public Vector3 Right
        {
            get
            {
                Get_InternalRight(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the camera's up vector.
        /// </summary>
        public Vector3 Up
        {
            get
            {
                Get_InternalUp(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the main camera.
        /// </summary>
        /// <returns></returns>
        public static Camera MainCamera
        {
            get
            {
                Get_InternalMainCamera(out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetAsCurrent(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLookAt(System.IntPtr nativePtr, Vector3 lookAt);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Camera InternalCreate();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalBoundingFrustum(System.IntPtr nativePtr, out BoundingFrustum data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalProjection(System.IntPtr nativePtr, out Matrix data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalView(System.IntPtr nativePtr, out Matrix data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalViewProjection(System.IntPtr nativePtr, out Matrix data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalFov(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalFov(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalNearPlane(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalNearPlane(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalFarPlane(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalFarPlane(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalForward(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRight(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalUp(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMainCamera(out Camera data);
    }
}