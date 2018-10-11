// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common.Entities;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Common.Components
{
    public struct TransformComponent : IComponent
    {
        public Vector3 Position;
        public Vector3 Rotation;

        public uint ComponentTypeId => 0u;
    }
}
