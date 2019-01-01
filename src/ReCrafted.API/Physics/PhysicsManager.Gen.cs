// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/24/2018 20:23:09 Source: 'PhysicsManager.API.cpp' Target: 'Physics/PhysicsManager.Gen.cs'

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
	/// <summary>
	///	PhysicsManager class
	/// </summary>
	public static partial class PhysicsManager 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool RayCast(ref Vector3 origin, ref Vector3 direction, float maxDistance, out RayCastHit hit, uint collisionLayers);

	}
}
