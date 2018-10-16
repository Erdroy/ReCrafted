// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    public partial class World
    {
        /// <summary>
        /// Adds given system to this world.
        /// </summary>
        /// <typeparam name="TSystem">The system type.</typeparam>
        public TSystem AddSystem<TSystem>() where TSystem : ComponentSystem, new()
        {
            var system = ComponentSystem.Create<TSystem>();
            AddSystem(NativePtr, system.NativePtr, system.SystemTypeId);
            return system;
        }

        /// <summary>
        /// Removes given system from this world.
        /// </summary>
        /// <param name="componentSystem">The system instance that will be removed from this world.</param>
        public void RemoveSystem(ComponentSystem componentSystem)
        {
            RemoveSystem(NativePtr, componentSystem.SystemTypeId);
        }

        /// <summary>
        /// Checks if given system type is already added to the world.
        /// </summary>
        /// <typeparam name="TSystem">The system type.</typeparam>
        /// <returns>True when added.</returns>
        public bool HasSystem<TSystem>() where TSystem : ComponentSystem, new()
        {
            return HasSystem(NativePtr, new TSystem().SystemTypeId);
        }

        /// <summary>
        /// Creates new Entity.
        /// To release this Entity, call 'entity.Destroy'.
        /// </summary>
        /// <returns>The created entity.</returns>
        public Entity CreateEntity()
        {
            return EntityInternals.CreateEntity(NativePtr);
        }
    }
}
