// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Physics
{
    public partial class RigidBodyActor
    {
        /// <summary>
        /// Adds given force to this actor movement.
        /// </summary>
        /// <param name="force">The force.</param>
        /// <param name="mode">The force mode.</param>
        /// <param name="autoAwake">Sets whatever the actor should be awaken.</param>
        public void AddForce(Vector3 force, ForceMode mode, bool autoAwake = true)
        {
            Internal_AddForce(NativePtr, ref force, (int)mode, autoAwake);
        }

        /// <summary>
        /// Adds given torque to this actor movement.
        /// </summary>
        /// <param name="torque">The torque.</param>
        /// <param name="mode">The force mode.</param>
        /// <param name="autoAwake">Sets whatever the actor should be awaken.</param>
        public void AddTorque(Vector3 torque, ForceMode mode, bool autoAwake = true)
        {
            Internal_AddTorque(NativePtr, ref torque, (int)mode, autoAwake);
        }

        /// <summary>
        /// Attaches given collision to this actor.
        /// </summary>
        /// <param name="collision">The collision that will be attached to this actor.</param>
        public void AttachCollision(Collision collision)
        {
            Internal_AttachCollision(NativePtr, collision.NativePtr);
        }

        /// <summary>
        /// Detaches the current collision from this actor.
        /// </summary>
        public void DetachCollision()
        {
            Internal_DetachCollision(NativePtr);
        }

        /// <summary>
        /// Creates new actor of type 'RigidBodyActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
        public static RigidBodyActor Create(bool dynamic = true)
        {
            return dynamic ? CreateDynamic() : CreateStatic();
        }
    }
}
