// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Core.Actors
{
    public partial class ActorBase
    {
        /// <summary>
        /// Adds given script type to this actor. Once script is added to this actor, it cannot be added anywhere else.
        /// </summary>
        /// <typeparam name="TScript">The script type. Must be derived from <see cref="Script"/> class.</typeparam>
        public TScript AddScript<TScript>() where TScript : Script, new()
        {
            // Create script instance
            var script = NewGeneric<TScript>(typeof(Script));

            // Add and return script
            InternalAddScript(NativePtr, script.NativePtr);

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
    }
}
