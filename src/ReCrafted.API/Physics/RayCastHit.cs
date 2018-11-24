// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Physics
{
    public struct RayCastHit
    {
        public Vector3 Point;
        public Vector3 Normal;
        public float Distance;
        public uint FaceIndex;
    }
}
