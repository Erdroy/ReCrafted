// ReCrafted © 2016-2017 Always Too Late

namespace ReCrafted.API.Common
{
    public partial class Script
    {
        /// <summary>
        /// Called when script is created and fully registered.
        /// </summary>
        protected internal virtual void OnCreate() { }

        /// <summary>
        /// Called every game frame.
        /// </summary>
        protected internal virtual void OnUpdate() { }

        /// <summary>
        /// Called every physics frame.
        /// </summary>
        protected internal virtual void OnSimulate() { }

        /// <summary>
        /// Called at the end of life of this script.
        /// </summary>
        protected internal virtual void OnDestroy() { }
        
        /// <summary>
        /// The instance of the entity that owns this script.
        /// </summary>
        public Entity Entity { get; internal set; }
    }
}
