// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core.Actors 
{
    public abstract class ActorBase : Object
    {

        public void SetParent(ActorBase newParent)
        {
            InternalSetParent(NativePtr, newParent.NativePtr);
        }

        public void AddChild(ActorBase child)
        {
            InternalAddChild(NativePtr, child.NativePtr);
        }

        public void RemoveChild(ActorBase child)
        {
            InternalRemoveChild(NativePtr, child.NativePtr);
        }

        public void SetActive(bool active)
        {
            InternalSetActive(NativePtr, active);
        }

        public void SetName(string name)
        {
            InternalSetName(NativePtr, name);
        }

        public string GetName()
        {
            return InternalGetName(NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetParent(System.IntPtr nativePtr, System.IntPtr newParent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetActive(System.IntPtr nativePtr, bool active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetName(System.IntPtr nativePtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string InternalGetName(System.IntPtr nativePtr);
    }
}