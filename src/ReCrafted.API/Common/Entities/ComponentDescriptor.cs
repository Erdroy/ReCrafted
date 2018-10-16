// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Entities
{
    /// <summary>
    /// ECS ComponentDescriptor class. Describes given component type by creating it's prototype and 
    /// implements GetRef function that can be used to cast native ComponentData pointer to reference
    /// with type of <see cref="TComponent"/>.
    /// </summary>
    /// <typeparam name="TComponent">The type of the component.</typeparam>
    public sealed unsafe class ComponentDescriptor<TComponent> : IComponentDescriptor where TComponent : IComponent, new()
    {
        private readonly TComponent _prototype;

        /// <summary>
        /// Default constructor.
        /// </summary>
        public ComponentDescriptor()
        {
            _prototype = new TComponent();
        }

        /// <summary>
        /// Gets reference with type of <see cref="TComponent"/> from given native component data pointer.
        /// </summary>
        /// <param name="data">The native component data pointer.</param>
        /// <returns>The reference to native pointer with type of <see cref="TComponent"/>.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public ref TComponent GetRef(ComponentData* data)
        {
            return ref Unsafe.AsRef<TComponent>(data);
        }
        
        /// <summary>
        /// The described component id.
        /// </summary>
        public ushort ComponentId => _prototype.ComponentTypeId;
        
        /// <summary>
        /// True when described component is flagged as native.
        /// </summary>
        public bool IsNativeComponent => _prototype.IsNativeComponent;
    }
}
