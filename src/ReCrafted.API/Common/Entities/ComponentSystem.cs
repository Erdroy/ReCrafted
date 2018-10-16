// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    public partial class ComponentSystem
    {
        /// <summary>
        /// Adds the given component type as required.
        /// When entity will have all required components, it will be present in the array returned by GetEntities.
        /// </summary>
        /// <typeparam name="TComponent">The required component type.</typeparam>
        /// <remarks>This function call is only valid in Initialize function.</remarks>
        protected void RequireComponent<TComponent>() where TComponent : IComponent, new()
        {
            var componentInstance = new TComponent();
            RequireComponent(NativePtr, componentInstance.ComponentTypeId, componentInstance.IsNativeComponent);
        }

        /// <summary>
        /// Adds the given component type as exclusion.
        /// When entity will this component type, it will NOT be present in the array returned by GetEntities.
        /// </summary>
        /// <typeparam name="TComponent">The excluded component type.</typeparam>
        /// <remarks>This function call is only valid in Initialize function.</remarks>
        protected void ExcludeComponent<TComponent>() where TComponent : IComponent, new()
        {
            var componentInstance = new TComponent();
            ExcludeComponent(NativePtr, componentInstance.ComponentTypeId, componentInstance.IsNativeComponent);
        }

        /// <summary>
        /// Gets entity enumerator.
        /// </summary>
        /// <returns>The entity enumerator.</returns>
        /// <remarks>Only valid in Update function.</remarks>
        protected EntityEnumerable GetEntities()
        {
            unsafe
            {
                var ptrEntity = GetEntities(NativePtr, out var numEntity);
                return new EntityEnumerable(ptrEntity, numEntity);
            }
        }

        /// <summary>
        /// Creates system of given type.
        /// </summary>
        /// <typeparam name="T">The system type that will be used.</typeparam>
        /// <returns>The created system of given type '<see cref="T"/>'.</returns>
        public static T Create<T>() where T : ComponentSystem, new()
        {
            return (T) Create(typeof(T).TypeHandle.Value);
        }
        
        public abstract ushort SystemTypeId { get; }
    }
}
