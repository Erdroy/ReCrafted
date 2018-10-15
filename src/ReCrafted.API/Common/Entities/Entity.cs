// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Runtime.InteropServices;

namespace ReCrafted.API.Common.Entities
{
    [StructLayout(LayoutKind.Explicit, Size = 16)]
    public struct Entity
    {
        [FieldOffset(0)]
        private readonly uint EntityId;

        [FieldOffset(8)]
        private readonly IntPtr WorldPtr;

        internal Entity(uint id, IntPtr worldPtr)
        {
            EntityId = id;
            WorldPtr = worldPtr;
        }

        public void AddComponent<TComponent>() where TComponent : IComponent, new()
        {
            AddComponent(new TComponent());
        }

        public void AddComponent<TComponent>(TComponent component) where TComponent : IComponent, new()
        {
            var componentHandle = GCHandle.Alloc(component, GCHandleType.Pinned);

            var componentPtr = componentHandle.AddrOfPinnedObject();
            var componentSize = (uint)Marshal.SizeOf<TComponent>();

            EntityInternals.AddEntityComponent(WorldPtr, EntityId, componentPtr, componentSize, component.ComponentTypeId, component.IsNativeComponent);

            componentHandle.Free();
        }
        
        public unsafe ComponentData* GetComponent(ushort componentId, bool isNativeComponent)
        {
            // Download component data
            return EntityInternals.GetEntityComponent(WorldPtr, EntityId, componentId, isNativeComponent);
        }

        public void RemoveComponent<TComponent>() where TComponent : IComponent, new()
        {
            var componentPrototype = new TComponent();
            var componentId = componentPrototype.ComponentTypeId;
            var nativeComponentId = componentPrototype.IsNativeComponent;
            EntityInternals.RemoveEntityComponent(WorldPtr, EntityId, componentId, nativeComponentId);
        }

        /// <summary>
        /// Removes all components from this entity.
        /// </summary>
        public void CleanComponents()
        {
            EntityInternals.CleanEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Activates this entity.
        /// </summary>
        public void Activate()
        {
            EntityInternals.ActivateEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Deactivates this entity.
        /// </summary>
        public void Deactivate()
        {
            EntityInternals.DeactivateEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Destroys this entity.
        /// </summary>
        public void Destroy()
        {
            EntityInternals.DestroyEntity(WorldPtr, EntityId);
        }
    }
}
