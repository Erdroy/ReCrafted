// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/25/2018 14:30:53 Source: 'RigidBodyActor.API.cpp' Target: 'Physics/RigidBodyActor.Gen.cs'

using ReCrafted.API.Common;
using ReCrafted.API.Common.Actors;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
	/// <summary>
	///	RigidBodyActor actor
	/// </summary>
	public partial class RigidBodyActor : ActorBase
	{
        internal RigidBodyActor() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddForce(IntPtr nativePtr, ref Vector3 force, int modeMode, bool autoAwake);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddTorque(IntPtr nativePtr, ref Vector3 torque, int modeMode, bool autoAwake);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AttachCollision(IntPtr nativePtr, IntPtr collisionNativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_DetachCollision(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern RigidBodyActor CreateDynamic();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern RigidBodyActor CreateStatic();

		/// <summary>
		///	Gets or sets rigid body's current velocity.
		/// </summary>
		public Vector3 Velocity 
		{
			get
			{
				Vector3 result;
				Internal_Velocity_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_Velocity_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current angular velocity.
		/// </summary>
		public Vector3 AngularVelocity 
		{
			get
			{
				Vector3 result;
				Internal_AngularVelocity_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_AngularVelocity_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current centre of mass.
		/// </summary>
		public Vector3 CentreOfMass 
		{
			get
			{
				Vector3 result;
				Internal_CentreOfMass_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_CentreOfMass_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current centre of mass.
		/// </summary>
		public float MaxAngularVelocity 
		{
			get
			{
				return Internal_MaxAngularVelocity_Get(NativePtr);
			}
			set
			{
				Internal_MaxAngularVelocity_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current linear damping.
		/// </summary>
		public float LinearDamping 
		{
			get
			{
				return Internal_LinearDamping_Get(NativePtr);
			}
			set
			{
				Internal_LinearDamping_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current angular damping.
		/// </summary>
		public float AngularDamping 
		{
			get
			{
				return Internal_AngularDamping_Get(NativePtr);
			}
			set
			{
				Internal_AngularDamping_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current mass.
		/// </summary>
		public float Mass 
		{
			get
			{
				return Internal_Mass_Get(NativePtr);
			}
			set
			{
				Internal_Mass_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's sleep state.
		/// </summary>
		public bool IsSleeping 
		{
			get
			{
				return Internal_IsSleeping_Get(NativePtr);
			}
			set
			{
				Internal_IsSleeping_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's kinematic state.
		/// </summary>
		public bool IsKinematic 
		{
			get
			{
				return Internal_IsKinematic_Get(NativePtr);
			}
			set
			{
				Internal_IsKinematic_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's CCD (Continuous Collision Detection) state.
		/// </summary>
		public bool ContinuousCollisionEnabled 
		{
			get
			{
				return Internal_ContinuousCollisionEnabled_Get(NativePtr);
			}
			set
			{
				Internal_ContinuousCollisionEnabled_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's dynamic state.
		/// </summary>
		public bool IsDynamic 
		{
			get
			{
				return Internal_IsDynamic_Get(NativePtr);
			}
		}

		/// <summary>
		///	Gets or sets rigid body's current collision layer.
		/// </summary>
		public uint CollisionLayer 
		{
			get
			{
				return Internal_CollisionLayer_Get(NativePtr);
			}
			set
			{
				Internal_CollisionLayer_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Velocity_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Velocity_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AngularVelocity_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AngularVelocity_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CentreOfMass_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CentreOfMass_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_MaxAngularVelocity_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_MaxAngularVelocity_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LinearDamping_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_LinearDamping_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AngularDamping_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_AngularDamping_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Mass_Set(IntPtr nativePtr, float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Mass_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_IsSleeping_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsSleeping_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_IsKinematic_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsKinematic_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ContinuousCollisionEnabled_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_ContinuousCollisionEnabled_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsDynamic_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CollisionLayer_Set(IntPtr nativePtr, uint value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint Internal_CollisionLayer_Get(IntPtr nativePtr);
	}
}
