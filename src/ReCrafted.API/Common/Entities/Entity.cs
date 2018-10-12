// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Runtime.InteropServices;

namespace ReCrafted.API.Common.Entities
{
    [StructLayout(LayoutKind.Explicit, Size = 16)]
    public struct Entity
    {
        [FieldOffset(0)]
        public uint EntityId;

        [FieldOffset(8)]
        public IntPtr WorldPtr;

        internal Entity(uint id, IntPtr worldPtr)
        {
            EntityId = id;
            WorldPtr = worldPtr;
        }
    }
}
