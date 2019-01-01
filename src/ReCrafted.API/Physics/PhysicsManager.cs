// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Physics
{
    public partial class PhysicsManager
    {
        public static bool RayCast(Vector3 position, Vector3 direction, float maxDistance = float.MaxValue, uint collisionLayer = 0)
        {
            return RayCast(ref position, ref direction, maxDistance, out _, collisionLayer);
        }

        public static bool RayCast(Vector3 position, Vector3 direction, out RayCastHit hit, float maxDistance = float.MaxValue, uint collisionLayer = 0)
        {
            return RayCast(ref position, ref direction, maxDistance, out hit, collisionLayer);
        }
    }
}
