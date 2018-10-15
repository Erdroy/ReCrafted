// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Runtime.InteropServices;

namespace ReCrafted.API.Common.Entities
{
    public sealed class ComponentHandle<TComponent> where TComponent : IComponent, new()
    {
        private GCHandle _handle;
        private IntPtr _nativeData;

        public ComponentHandle()
        {
            Data = new TComponent();
            _handle = GCHandle.Alloc(Data, GCHandleType.Pinned);
            DataPointer = _handle.AddrOfPinnedObject();
            DataSize = (uint)Marshal.SizeOf<TComponent>();
        }

        ~ComponentHandle()
        {
            _handle.Free();
        }

        internal void SetNativeDataPointer(IntPtr native)
        {

        }

        public void SetData<TType>(ref TType data, int offset) where TType : struct
        {
            unsafe
            {
                
            }
        }

        public IntPtr DataPointer { get; }
        public uint DataSize { get; }
        public TComponent Data { get; }
    }
}
