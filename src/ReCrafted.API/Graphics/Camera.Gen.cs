// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/18/2018 17:30:53 Source: 'Camera.API.cpp' Target: 'Graphics/Camera.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Camera class.
	/// </summary>
	public class Camera : Object
	{
        internal Camera() {}

		/// <summary>
		///	Creates new Camera
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Camera Create();

		/// <summary>
		///	Sets the camera as current.
		/// </summary>
		public void SetAsCurrent()
		{
			Internal_SetAsCurrent(NativePtr);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_SetAsCurrent(IntPtr nativePtr);

		/// <summary>
		///	Camera's Field of view.
		/// </summary>
		public float Fov 
		{
			get
			{
				return Internal_Fov_Get(NativePtr);
			}
			set
			{
				Internal_Fov_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Camera's position.
		/// </summary>
		public Vector3 Position 
		{
			get
			{
				Vector3 result;
				Internal_Position_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_Position_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Camera's rotation.
		/// </summary>
		public Quaternion Rotation 
		{
			get
			{
				Quaternion result;
				Internal_Rotation_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_Rotation_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Camera's bounding frustum
		/// </summary>
		public BoundingFrustum GetBoundingFrustum 
		{
			get
			{
				BoundingFrustum result;
				Internal_GetBoundingFrustum_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	Is camera's free movement enabled?
		/// </summary>
		public bool FreeMovement 
		{
			get
			{
				return Internal_FreeMovement_Get(NativePtr);
			}
			set
			{
				Internal_FreeMovement_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Far camera plane
		/// </summary>
		public float FarPlane 
		{
			get
			{
				return Internal_FarPlane_Get(NativePtr);
			}
			set
			{
				Internal_FarPlane_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Near camera plane
		/// </summary>
		public float NearPlane 
		{
			get
			{
				return Internal_NearPlane_Get(NativePtr);
			}
			set
			{
				Internal_NearPlane_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Camera's Z positive axis
		/// </summary>
		public Vector3 Forward 
		{
			get
			{
				Vector3 result;
				Internal_Forward_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	Camera's Y positive axis
		/// </summary>
		public Vector3 Up 
		{
			get
			{
				Vector3 result;
				Internal_Up_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	Camera's X positive axis
		/// </summary>
		public Vector3 Right 
		{
			get
			{
				Vector3 result;
				Internal_Right_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	The current camera.
		/// </summary>
		public static Camera Current 
		{
			get
			{
				return Internal_Current_Get();
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Fov_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Fov_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Position_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Position_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Rotation_Set(IntPtr nativePtr, ref Quaternion value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Rotation_Get(IntPtr nativePtr, out Quaternion result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_GetBoundingFrustum_Get(IntPtr nativePtr, out BoundingFrustum result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_FreeMovement_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_FreeMovement_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_FarPlane_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_FarPlane_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_NearPlane_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_NearPlane_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Forward_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Up_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Right_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Camera Internal_Current_Get();
	}
}
