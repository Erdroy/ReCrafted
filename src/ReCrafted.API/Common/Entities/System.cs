
namespace ReCrafted.API.Common.Entities
{
    public partial class System
    {
        /// <summary>
        /// Creates system of given type.
        /// </summary>
        /// <typeparam name="T">The system type that will be used.</typeparam>
        /// <returns>The created system of given type '<see cref="T"/>'.</returns>
        public static T Create<T>() where T : System, new()
        {
            return (T) Create(typeof(T).TypeHandle.Value);
        }

        public abstract ushort SystemTypeId { get; }
    }
}
