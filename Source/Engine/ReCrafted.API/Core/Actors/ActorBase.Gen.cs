// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core.Actors 
{
    public abstract class ActorBase : Object
    {

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
        
        public ActorBase Parent
        {
            get
            {
                Get_InternalParent(NativePtr, out var data);
                return data;
            }
            set => Set_InternalParent(NativePtr, ref value.NativePtr);
        }
        
        public Vector3 Position
        {
            get
            {
                Get_InternalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalPosition(NativePtr, ref value);
        }
        
        public Vector3 LocalPosition
        {
            get
            {
                Get_InternalLocalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalPosition(NativePtr, ref value);
        }
        
        public Quaternion Rotation
        {
            get
            {
                Get_InternalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRotation(NativePtr, ref value);
        }
        
        public Quaternion LocalRotation
        {
            get
            {
                Get_InternalLocalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalRotation(NativePtr, ref value);
        }
        
        public Vector3 Scale
        {
            get
            {
                Get_InternalScale(NativePtr, out var data);
                return data;
            }
            set => Set_InternalScale(NativePtr, ref value);
        }
        
        public Vector3 LocalScale
        {
            get
            {
                Get_InternalLocalScale(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLocalScale(NativePtr, ref value);
        }
        
        public Transform Transform
        {
            get
            {
                Get_InternalTransform(NativePtr, out var data);
                return data;
            }
            set => Set_InternalTransform(NativePtr, ref value);
        }
        
        public bool IsActiveSelf
        {
            get
            {
                Get_InternalIsActiveSelf(NativePtr, out var data);
                return data;
            }
        }
        
        public bool IsActive
        {
            get
            {
                Get_InternalIsActive(NativePtr, out var data);
                return data;
            }
        }
        
        public string Name
        {
            get
            {
                Get_InternalName(NativePtr, out var data);
                return data;
            }
            set => Set_InternalName(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveChild(System.IntPtr nativePtr, System.IntPtr child);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetActive(System.IntPtr nativePtr, bool active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalParent(System.IntPtr nativePtr, out ActorBase data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalParent(System.IntPtr nativePtr, ref System.IntPtr data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalScale(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalScale(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLocalScale(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLocalScale(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalTransform(System.IntPtr nativePtr, out Transform data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalTransform(System.IntPtr nativePtr, ref Transform data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsActiveSelf(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsActive(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalName(System.IntPtr nativePtr, ref string data);
    }
}