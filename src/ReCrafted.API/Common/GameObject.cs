// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.CompilerServices;
using ReCrafted.API.Common.Components;

namespace ReCrafted.API.Common
{
    public partial class GameObject
    {
        private GameObject() { }
        
        /// <summary>
        /// Adds new script by given type <see cref="TScript"/>.
        /// </summary>
        /// <typeparam name="TScript">The script type, must inherit from Script.</typeparam>
        /// <returns>The newly created script instance.</returns>
        public TScript AddScript<TScript>() where TScript : Script, new()
        {
            var script = (TScript)NewGeneric<Script>(new TScript());

            Internal_AddScript(NativePtr, script.NativePtr);

            return script;
        }

        /// <summary>
        /// Removes given script instance.
        /// </summary>
        /// <param name="script">The script that will be removed.</param>
        public void RemoveScript(Script script)
        {
            Internal_RemoveScript(NativePtr, script.NativePtr);
        }

        /// <summary>
        /// Sets given game object as children of this game object.
        /// </summary>
        /// <param name="gameObject">The game object that will become children of this game object.</param>
        public void AddChild(GameObject gameObject)
        {
            Internal_AddChildren(NativePtr, gameObject.NativePtr);
        }

        /// <summary>
        /// Removes given game object from this game object.
        /// </summary>
        /// <param name="gameObject">The game object that will be alone, in the world of game objects...</param>
        public void RemoveChild(GameObject gameObject)
        {
            Internal_RemoveChildren(NativePtr, gameObject.NativePtr);
        }

        /// <summary>
        /// Gets child of given id. If index is invalid, 
        /// eg.: less than 0 or out of children array bounds,
        /// returned game object will be null!
        /// </summary>
        /// <param name="index">The children index. Must be >= 0 and less than <see cref="ChildCount"/></param>
        /// <returns>Child with specified index or null.</returns>
        public GameObject GetChild(int index)
        {
            return Internal_GetChild(NativePtr, index);
        }

        /// <summary>
        ///	Gets the number of children of this game objects.
        /// </summary>
        public int ChildCount => Internal_GetChildCount(NativePtr);

        /// <summary>
        /// Creates new game object.
        /// </summary>
        /// <returns>Newly created game object.</returns>
        public static GameObject New()
        {
            return New<GameObject>();
        }

        /// <summary>
        /// Contains the reference to transform owned by this game object.
        /// </summary>
        public unsafe ref TransformComponent Transform => ref Unsafe.AsRef<TransformComponent>(Internal_GetTransform(NativePtr));
    }
}
