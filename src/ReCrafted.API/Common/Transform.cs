// ReCrafted (c) 2016-2018 Always Too Late

using System;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Common
{
    public struct Transform : IEquatable<Transform>
    {
        public Vector3 Position;
        public Quaternion Rotation;
        public Vector3 Scale;

        // TODO: API Transform implementation

        public bool Equals(Transform other)
        {
            return Position.Equals(other.Position) && Rotation.Equals(other.Rotation) && Scale.Equals(other.Scale);
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(null, obj)) return false;
            return obj is Transform transform && Equals(transform);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = Position.GetHashCode();
                hashCode = (hashCode * 397) ^ Rotation.GetHashCode();
                hashCode = (hashCode * 397) ^ Scale.GetHashCode();
                return hashCode;
            }
        }
    }
}
