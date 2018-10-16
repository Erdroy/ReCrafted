// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ReCrafted.API.Common.Entities
{
    /// <summary>
    /// ECS Entity structure.
    /// </summary>
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

        /// <summary>
        /// Adds component of given type to this entity.
        /// </summary>
        /// <typeparam name="TComponent">The component type which will be removed.</typeparam>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void AddComponent<TComponent>() where TComponent : IComponent, new()
        {
            AddComponent(new TComponent());
        }
        
        /// <summary>
        /// Adds given component to this entity.
        /// </summary>
        /// <param name="component">The component instance.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void AddComponent<TComponent>(TComponent component) where TComponent : IComponent, new()
        {
            var componentHandle = GCHandle.Alloc(component, GCHandleType.Pinned);

            var componentPtr = componentHandle.AddrOfPinnedObject();
            var componentSize = (uint)Marshal.SizeOf<TComponent>();

            EntityInternals.AddEntityComponent(WorldPtr, EntityId, componentPtr, componentSize, component.ComponentTypeId, component.IsNativeComponent);

            componentHandle.Free();
        }
        
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public unsafe ref TComponent GetComponent<TComponent>(ref ComponentDescriptor<TComponent> descriptor) where TComponent : IComponent, new()
        {
            // Get entity component data pointer.
            var nativePointer = EntityInternals.GetEntityComponent(WorldPtr, EntityId, descriptor.ComponentTypeId, descriptor.IsNativeComponent);
            
            // Get reference to the pointer.
            return ref descriptor.GetRef(nativePointer);
        }

        /// <summary>
        /// Removes component of given type from this entity.
        /// </summary>
        /// <typeparam name="TComponent">The component type which will be removed.</typeparam>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
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
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void CleanComponents()
        {
            EntityInternals.CleanEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Activates this entity.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Activate()
        {
            EntityInternals.ActivateEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Deactivates this entity.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Deactivate()
        {
            EntityInternals.DeactivateEntity(WorldPtr, EntityId);
        }

        /// <summary>
        /// Destroys this entity.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void Destroy()
        {
            EntityInternals.DestroyEntity(WorldPtr, EntityId);
        }
    }
}
