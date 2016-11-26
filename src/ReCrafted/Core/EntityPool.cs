// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;

namespace ReCrafted.Core
{
    /// <summary>
    /// EntityPool class.
    /// </summary>
    public static class EntityPool
    {
        private static readonly List<IEntity> Entities = new List<IEntity>();

        /// <summary>
        /// Ticks all entities.
        /// </summary>
        internal static void Tick()
        {
            foreach (var entity in Entities)
            {
                entity.Tick();
            }
        }

        /// <summary>
        /// Simulates all entities.
        /// </summary>
        internal static void Simulate()
        {
            foreach (var entity in Entities)
            {
                entity.Simulate();
            }
        }

        /// <summary>
        /// Draws all entities.
        /// </summary>
        internal static void Draw()
        {
            foreach (var entity in Entities)
            {
                entity.Draw();
            }
        }

        /// <summary>
        /// Destroy entity which has the given entity id.
        /// </summary>
        /// <param name="entityId"></param>
        internal static void Destroy(int entityId)
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Creates new entity and assigns given name.
        /// </summary>
        /// <param name="entityName">The entity name(optional).</param>
        /// <returns>The new entity.</returns>
        public static Entity CreateEntity(string entityName = "<unnamed>")
        {
            var entity = new Entity
            {
                Name = entityName,
                EntityId = Entities.Count
            };

            entity.Init();
            
            Entities.Add(entity);
            return entity;
        }
    }
}