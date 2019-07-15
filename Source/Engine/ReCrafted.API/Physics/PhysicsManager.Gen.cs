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
        
        /// <summary>
        ///     Creates new scene for physics simulation.
        /// </summary>
        public static PhysicsScene CreateScene()
        {
            return InternalCreateScene();
        }
        
        /// <summary>
        ///     Gets physics scene that contains given world position in it's bounds.
        ///     TODO: Add double-based math to API.
        /// </summary>
        public static PhysicsScene GetSceneAt(Vector3 worldPosition)
        {
            return InternalGetSceneAt(worldPosition);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern PhysicsScene InternalCreateScene();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern PhysicsScene InternalGetSceneAt(Vector3 worldPosition);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalRayCast(Vector3 position, Vector3 direction, float maxDistance, ref RayCastHit hit, uint collisionLayer);
    }
}