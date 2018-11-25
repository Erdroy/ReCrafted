// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/25/2018 14:35:29 Source: 'Collision.API.cpp' Target: 'Physics/Collision.Gen.cs'

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
	/// <summary>
	///	Collision class
	/// </summary>
	public partial class Collision : Object
	{
        internal Collision() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Collision CreateBoxCollision(ref Vector3 extents);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Collision CreateSphereCollision(float radius);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Collision CreateCapsuleCollision(float radius, float halfHeight);

	}
}
