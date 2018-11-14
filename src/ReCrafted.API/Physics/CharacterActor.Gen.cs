// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/14/2018 14:53:40 Source: 'CharacterActor.API.cpp' Target: 'Physics/CharacterActor.Gen.cs'

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
	/// <summary>
	///	CharacterActor actor
	/// </summary>
	public partial class CharacterActor : ActorBase
	{
        internal CharacterActor() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern CharacterCollisionFlags Internal_MoveCharacter(IntPtr nativePtr, ref Vector3 displacement);

		/// <summary>
		///	Gets or sets controller's contact offset.
		/// </summary>
		public float ContactOffset 
		{
			get
			{
				return Internal_ContactOffset_Get(NativePtr);
			}
			set
			{
				Internal_ContactOffset_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets the slope limit.
		/// </summary>
		public float SlopeLimit 
		{
			get
			{
				return Internal_SlopeLimit_Get(NativePtr);
			}
			set
			{
				Internal_SlopeLimit_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets the step offset.
		/// </summary>
		public float StepOffset 
		{
			get
			{
				return Internal_StepOffset_Get(NativePtr);
			}
			set
			{
				Internal_StepOffset_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets controller's height.
		/// </summary>
		public float Height 
		{
			get
			{
				return Internal_Height_Get(NativePtr);
			}
			set
			{
				Internal_Height_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets controller's radius.
		/// </summary>
		public float Radius 
		{
			get
			{
				return Internal_Radius_Get(NativePtr);
			}
			set
			{
				Internal_Radius_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets controller's up direction.
		/// </summary>
		public Vector3 UpDirection 
		{
			get
			{
				Vector3 result;
				Internal_UpDirection_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_UpDirection_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets controller's current velocity.
		/// </summary>
		public Vector3 Velocity 
		{
			get
			{
				Vector3 result;
				Internal_Velocity_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	Gets controller's grounded flag from last Move call.
		/// </summary>
		public bool IsGrounded 
		{
			get
			{
				return Internal_IsGrounded_Get(NativePtr);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ContactOffset_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_ContactOffset_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_SlopeLimit_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_SlopeLimit_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_StepOffset_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_StepOffset_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Height_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Height_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Radius_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Radius_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_UpDirection_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_UpDirection_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Velocity_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsGrounded_Get(IntPtr nativePtr);
	}
}
