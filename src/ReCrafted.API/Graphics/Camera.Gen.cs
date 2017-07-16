// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/16/2017 15:04:07 Source: 'Object.API.cpp' Target: 'Graphics/Camera.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Camera class.
	/// </summary>
	public class Camera : Object
	{
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
			get { return Internal_Fov_Get(NativePtr); }
			set { Internal_Fov_Set(NativePtr, value); }
		}

		/// <summary>
		///	The current camera.
		/// </summary>
		public static Camera Current => Internal_Current_Get();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Fov_Set(IntPtr ptr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Fov_Get(IntPtr ptr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Camera Internal_Current_Get();
	}
}
