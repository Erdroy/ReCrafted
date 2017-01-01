// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Core
{
    /// <summary>
    /// Entity class.
    /// </summary>
    public sealed class Entity : IEntity
    {
        /// <summary>
        /// The entity id.
        /// </summary>
        internal int EntityId;

        /// <summary>
        /// Entity constructor.
        /// </summary>
        internal Entity()
        {
            Position = Vector3.Zero;
            Forward = Vector3.ForwardLH;
            Rotation = Quaternion.Identity;
            EulerAngles = Vector3.Zero;
        }

        /// <summary>
        /// Initializes the entity.
        /// </summary>
        public void Init()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Ticks the entity.
        /// </summary>
        public void Tick()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Simulates the entity.
        /// </summary>
        public void Simulate()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Draws the entity.
        /// </summary>
        public void Draw()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// Destroys the entity.
        /// </summary>
        public void Destroy()
        {
            EntityPool.Destroy(EntityId);
        }

        /// <summary>
        /// Disposes the entity.
        /// </summary>
        public void Dispose()
        {
            throw new System.NotImplementedException();
        }

        /// <summary>
        /// The Entity name.
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// The entity position.
        /// </summary>
        public Vector3 Position { get; set; }

        /// <summary>
        /// The entity direction.
        /// </summary>
        public Vector3 Forward { get; set; }

        /// <summary>
        /// Entity rotation.
        /// </summary>
        public Vector3 EulerAngles { get; set; }

        /// <summary>
        /// Entity rotation.
        /// </summary>
        public Quaternion Rotation { get; set; }

        /// <summary>
        /// Transform matrix.
        /// </summary>
        public Matrix Transform { get; set; }
    }
}