// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Common
{
    /// <summary>
    ///     Transform structure used by Actors in ReCrafted.API.
    /// </summary>
    public struct Transform : IEquatable<Transform>
    {
        /// <summary>
        ///     The translation of this transform.
        /// </summary>
        public Vector3 Translation;

        /// <summary>
        ///     The orientation of this transform.
        /// </summary>
        public Quaternion Orientation;

        /// <summary>
        ///     The scale of this transform.
        /// </summary>
        public Vector3 Scale;

        // TODO: API Transform implementation

        /// <summary>
        ///     Transform given point direction using this transform.
        /// </summary>
        /// <param name="point">The point vector.</param>
        /// <returns>The transformed point.</returns>
        public Vector3 TransformDirection(Vector3 point)
        {
            return Vector3.Transform(point, Orientation);
        }

        public bool Equals(Transform other)
        {
            return Translation.Equals(other.Translation) && Orientation.Equals(other.Orientation) &&
                   Scale.Equals(other.Scale);
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
                var hashCode = Translation.GetHashCode();
                hashCode = (hashCode * 397) ^ Orientation.GetHashCode();
                hashCode = (hashCode * 397) ^ Scale.GetHashCode();
                return hashCode;
            }
        }

        /// <summary>
        ///     Gets the translation matrix from this transform.
        /// </summary>
        public Matrix TranslationMatrix => Matrix.Translation(Translation);

        /// <summary>
        ///     Gets the scaling matrix from this transform.
        /// </summary>
        public Matrix ScalingMatrix => Matrix.Scaling(Scale);

        /// <summary>
        ///     Gets the rotation matrix from this transform.
        /// </summary>
        public Matrix RotationMatrix => Matrix.RotationQuaternion(Orientation);

        /// <summary>
        ///     Gets the transformation matrix from this transform.
        /// </summary>
        public Matrix TransformMatrix => ScalingMatrix * RotationMatrix * TranslationMatrix;

        /// <summary>
        ///     Gets the forward directional vector from this transform.
        /// </summary>
        public Vector3 Forward => RotationMatrix.ForwardLH;

        /// <summary>
        ///     Gets the backward directional vector from this transform.
        /// </summary>
        public Vector3 Backward => RotationMatrix.BackwardLH;

        /// <summary>
        ///     Gets the up directional vector from this transform.
        /// </summary>
        public Vector3 Up => RotationMatrix.Up;

        /// <summary>
        ///     Gets the down directional vector from this transform.
        /// </summary>
        public Vector3 Down => RotationMatrix.Down;

        /// <summary>
        ///     Gets the left directional vector from this transform.
        /// </summary>
        public Vector3 Left => RotationMatrix.Left;

        /// <summary>
        ///     Gets the right directional vector from this transform.
        /// </summary>
        public Vector3 Right => RotationMatrix.Right;
    }
}