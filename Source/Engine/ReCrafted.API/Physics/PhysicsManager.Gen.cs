// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     PhysicsManager class. Provides basic physics functionality.
    /// </summary>
    public static partial class PhysicsManager
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalRayCast(Vector3 position, Vector3 direction, float maxDistance, ref RayCastHit hit, uint collisionLayer);
    }
}