// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.CompilerServices;
using ReCrafted.API.Common.Components;
using ReCrafted.API.Common.Entities;
using ReCrafted.API.Core;

namespace ReCrafted.API.Common
{
    public partial class GameObject
    {
        private Entity _entity;

        private GameObject() { }

        private void InitializeEntity()
        {
            _entity = new Entity(Internal_GetEntityId(NativePtr), World.GetMainWorld().NativePtr);
        }

        /// <summary>
        /// Adds given component to entity owned by this game object.
        /// </summary>
        /// <param name="component">The component instance.</param>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void AddComponent<TComponent>(TComponent component) where TComponent : IComponent, new()
        {
            if (component is TransformComponent)
                throw new ReCraftedException("TransformComponent is automatically added to the game object so, " +
                                             "there is no need to add it manually.");

            if (_entity.Empty())
                InitializeEntity();

            _entity.AddComponent(component);
        }

        /// <summary>
        /// Gets component of given type from entity owned by this game object.
        /// </summary>
        /// <typeparam name="TComponent">The component type.</typeparam>
        /// <param name="descriptor">The component descriptor.</param>
        /// <returns>The reference to component.</returns>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public ref TComponent GetComponent<TComponent>(ref ComponentDescriptor<TComponent> descriptor) where TComponent : IComponent, new()
        {
            if (_entity.Empty())
                InitializeEntity();

            return ref _entity.GetComponent(ref descriptor);
        }

        /// <summary>
        /// Removes component of given type from entity owned by this game object.
        /// </summary>
        /// <typeparam name="TComponent">The component type which will be removed.</typeparam>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void RemoveComponent<TComponent>() where TComponent : IComponent, new()
        {
            if (_entity.Empty())
                InitializeEntity();

            _entity.RemoveComponent<TComponent>();
        }

        /// <summary>
        /// Removes all components from entity owned by this game object.
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public void CleanComponents()
        {
            if (_entity.Empty())
                InitializeEntity();

            _entity.CleanComponents();
        }

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
