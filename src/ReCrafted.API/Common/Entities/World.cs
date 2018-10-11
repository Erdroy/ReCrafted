// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Entities
{
    public partial class World
    {
        /// <summary>
        /// Adds given system to this world.
        /// </summary>
        /// <param name="system">The system instance that will be added to this world.</param>
        public void AddSystem(System system)
        {
            AddSystem(NativePtr, system.NativePtr, system.SystemTypeId);
        }
    }
}
