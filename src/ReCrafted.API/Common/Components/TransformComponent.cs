// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.InteropServices;
using ReCrafted.API.Common.Entities;
using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Common.Components
{
    /// <summary>
    /// (native) Transform component.
    /// </summary>
    [StructLayout(LayoutKind.Explicit)]
    public struct TransformComponent : IComponent
    {
        /// <summary>
        /// The transform's position.
        /// </summary>
        [FieldOffset(0)]
        public Vector3 Position;

        /// <summary>
        /// The transform's rotation.
        /// </summary>
        [FieldOffset(12)]
        public Quaternion Rotation;

        public ushort ComponentTypeId => 0;
        public bool IsNativeComponent => true;
        public uint SizeInBytes => 28;

        private static ComponentDescriptor<TransformComponent> _descriptor;
        public static ref ComponentDescriptor<TransformComponent> Descriptor
        {
            get
            {
                if (_descriptor == null)
                    _descriptor = new ComponentDescriptor<TransformComponent>();
                return ref _descriptor;
            }
        }
    }
}
