// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Common
{
    public partial class ActorBase
    {
        /// <summary>
        /// Adds given script type to this actor. Once script is added to this actor, it cannot be added anywhere else.
        /// </summary>
        public TScript AddScript<TScript>() where TScript : Script, new()
        {
            // Create script instance
            var script = (TScript)NewGeneric<Script>(new TScript());

            // Add and return script
            Internal_AddScript(NativePtr, script.NativePtr);
            return script;
        }

        /// <summary>
        /// Transform given point direction using this transform.
        /// </summary>
        /// <param name="point">The point vector.</param>
        /// <returns>The transformed point.</returns>
        public Vector3 TransformDirection(Vector3 point)
        {
            return Vector3.Transform(point, Rotation);
        }

        /// <summary>
        /// Removes given script. Once script is removed, it get it's instance destroyed and cannot be used anymore.
        /// </summary>
        /// <param name="script">The script instance that will be removed from this actor.</param>
        public void RemoveScript(Script script)
        {
            Internal_RemoveScript(NativePtr, script.NativePtr);
        }

        /// <summary>
        /// Sets given actor as parent of this actor.
        /// </summary>
        /// <param name="parent">The new parent of this actor.</param>
        public void SetParent(ActorBase parent)
        {
            Internal_SetParent(NativePtr, parent.NativePtr);
        }

        /// <summary>
        /// Sets given actor as child of this actor,
        /// </summary>
        /// <param name="child">The actor that will become child of this actor.</param>
        public void AddChild(ActorBase child)
        {
            Internal_AddChild(NativePtr, child.NativePtr);
        }

        /// <summary>
        /// Removes given actor from being a child of this actor.
        /// </summary>
        /// <param name="child">The child actor that will be removed from this actor.</param>
        public void RemoveChild(ActorBase child)
        {
            Internal_RemoveChild(NativePtr, child.NativePtr);
        }

        /// <summary>
        /// Gets the current transform.
        /// Transform cannot be modified, you need to use properties (Position, LocalPosition, Rotation etc.) of the actor.
        /// </summary>
        public Transform Transform => Internal_GetTransform(NativePtr);
    }
}
