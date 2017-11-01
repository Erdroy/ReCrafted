// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Core;

namespace ReCrafted.API.Common
{
    public partial class Entity
    {
        /// <summary>
        /// Adds new script of given type to this entity.
        /// </summary>
        /// <typeparam name="T">The script type to be used.</typeparam>
        /// <returns>The added script instance of given type.</returns>
        public T AddScript<T>() where T : Script, new()
        {
            var script = new T();

            // Register the script
            Script.Register(script);

            if (script == null)
                throw new ReCraftedException("Could not create script!");

            // Set entity reference.
            script.Entity = this;

            // Add script to this entity
            Internal_AddScript(NativePtr, script);

            // Call 'OnCreate'
            script.OnCreate();

            return script;
        }

        /// <summary>
        /// Removes given script.
        /// Exception will throw when script is null or it is not owned by this entity.
        /// </summary>
        /// <param name="script">The script to be removed from this entity.</param>
        public void RemoveScript<T>(T script) where T : Script
        {
            if(script == null)
                throw new ReCraftedException("Script is not owned by this entity!");

            Internal_RemoveScript(NativePtr, script);
        }
    }
}
