// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.UI
{
    [Flags]
    public enum UIDirtyFlags
    {
        None = 0,
        VertexBuffer = 1 << 0,
        Layout = 1 << 1
    }
}
