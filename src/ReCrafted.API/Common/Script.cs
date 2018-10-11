// ReCrafted (c) 2016-2018 Always Too Late

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
        protected virtual void OnUpdate() { }

        /// <summary>
        /// Called every physics frame.
        /// </summary>
        protected virtual void OnSimulate() { }

        /// <summary>
        /// Called at the end of life of this script.
        /// </summary>
        protected virtual void OnDestroy() { }
        
        /// <summary>
        /// The instance of the entity that owns this script.
        /// </summary>
        //public Entity Entity { get; internal set; }
    }
}
