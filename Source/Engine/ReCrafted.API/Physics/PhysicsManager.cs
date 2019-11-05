// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Physics
{
    public partial class PhysicsManager
    {
        public static bool RayCast(Vector3 origin, Vector3 direction, float maxDistance, out RayCastHit hit, uint collisionLayer)
        {
            hit = default;
            return InternalRayCast(origin, direction, maxDistance, ref hit, collisionLayer);
        }
    }
}
