// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core.Actors;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     RigidBodyActor actor class.
    /// </summary>
    public abstract class RigidBodyActor : ActorBase
    {
        
        /// <summary>
        ///     Attaches given collider to this actor.
        /// </summary>
        /// <param name="collider">Collider to attach.</param>
        /// <param name="awake">
        ///     Awakes the actor after adding the collider to it.
        ///     Only available for dynamic RigidBodies.
        /// </param>
        public void AttachCollider(Collider collider, bool awake = false)
        {
            InternalAttachCollider(NativePtr, collider? collider.NativePtr : System.IntPtr.Zero, awake);
        }
        
        /// <summary>
        ///     Detaches given collider to this actor.
        /// </summary>
        /// <param name="collider">Collider to detach.</param>
        /// <param name="awakeOnLostTouch">
        ///     When this actors looses the touch with other collision objects, it will be awaken.
        /// </param>
        public void DetachCollider(Collider collider, bool awakeOnLostTouch = true)
        {
            InternalDetachCollider(NativePtr, collider? collider.NativePtr : System.IntPtr.Zero, awakeOnLostTouch);
        }
        
        /// <summary>
        ///     Detaches currently attached colliders from this actor.
        /// </summary>
        /// <param name="awakeOnLostTouch">
        ///     When this actors looses the touch with other collision objects, it will be awaken.
        /// </param>
        public void DetachColliders(bool awakeOnLostTouch = true)
        {
            InternalDetachColliders(NativePtr, awakeOnLostTouch);
        }
        
        /// <summary>
        ///     Adds force to this actor.
        /// </summary>
        /// <param name="force">The torque.</param>
        /// <param name="forceMode">The mode of the force.</param>
        /// <param name="awake">When true, this actor will be awaken, if sleeping.</param>
        public void AddForce(Vector3 force, ForceMode forceMode, bool awake = true)
        {
            InternalAddForce(NativePtr, force, forceMode, awake);
        }
        
        /// <summary>
        ///     Adds torque to this actor.
        /// </summary>
        /// <param name="torque">The torque.</param>
        /// <param name="forceMode">The mode of the force.</param>
        /// <param name="awake">When true, this actor will be awaken, if sleeping.</param>
        public void AddTorque(Vector3 torque, ForceMode forceMode, bool awake = true)
        {
            InternalAddTorque(NativePtr, torque, forceMode, awake);
        }
        
        /// <summary>
        ///     Creates new dynamic RigidBodyActor.
        /// </summary>
        public static RigidBodyActor Create()
        {
            return InternalCreate();
        }
        
        /// <summary>
        ///     Creates new dynamic RigidBodyActor.
        /// </summary>
        public static RigidBodyActor CreateDynamic()
        {
            return InternalCreateDynamic();
        }
        
        /// <summary>
        ///     Creates new static RigidBodyActor.
        /// </summary>
        public static RigidBodyActor CreateStatic()
        {
            return InternalCreateStatic();
        }

        /// <summary>
        ///     Gets the scene that this actor is attached to.
        /// </summary>
        public PhysicsScene Scene
        {
            get
            {
                Get_InternalScene(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the currently attached colliders.
        /// </summary>
        public Collider[] Colliders
        {
            get
            {
                Get_InternalColliders(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets or sets the sync mode of this actor.
        /// </summary>
        public RigidBodySyncMode SyncMode
        {
            get
            {
                Get_InternalSyncMode(NativePtr, out var data);
                return data;
            }
            set => Set_InternalSyncMode(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets world-space position of this actor.
        /// </summary>
        public override Vector3 Position
        {
            get
            {
                Get_InternalPosition(NativePtr, out var data);
                return data;
            }
            set => Set_InternalPosition(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets world-space position of this actor.
        /// </summary>
        public override Quaternion Rotation
        {
            get
            {
                Get_InternalRotation(NativePtr, out var data);
                return data;
            }
            set => Set_InternalRotation(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets gravity affection state. When true, this actor will be affected by gravitation.
        /// </summary>
        public bool AffectedByGravity
        {
            get
            {
                Get_InternalAffectedByGravity(NativePtr, out var data);
                return data;
            }
            set => Set_InternalAffectedByGravity(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the collision layer.
        /// </summary>
        public uint CollisionLayer
        {
            get
            {
                Get_InternalCollisionLayer(NativePtr, out var data);
                return data;
            }
            set => Set_InternalCollisionLayer(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the velocity.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public Vector3 Velocity
        {
            get
            {
                Get_InternalVelocity(NativePtr, out var data);
                return data;
            }
            set => Set_InternalVelocity(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the angular velocity.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public Vector3 AngularVelocity
        {
            get
            {
                Get_InternalAngularVelocity(NativePtr, out var data);
                return data;
            }
            set => Set_InternalAngularVelocity(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the centre of mass.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public Vector3 CentreOfMass
        {
            get
            {
                Get_InternalCentreOfMass(NativePtr, out var data);
                return data;
            }
            set => Set_InternalCentreOfMass(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the max of angular velocity.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public float MaxAngularVelocity
        {
            get
            {
                Get_InternalMaxAngularVelocity(NativePtr, out var data);
                return data;
            }
            set => Set_InternalMaxAngularVelocity(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the linear damping.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public float LinearDamping
        {
            get
            {
                Get_InternalLinearDamping(NativePtr, out var data);
                return data;
            }
            set => Set_InternalLinearDamping(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the angular damping.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public float AngularDamping
        {
            get
            {
                Get_InternalAngularDamping(NativePtr, out var data);
                return data;
            }
            set => Set_InternalAngularDamping(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the mass.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public float Mass
        {
            get
            {
                Get_InternalMass(NativePtr, out var data);
                return data;
            }
            set => Set_InternalMass(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the continuous collision detection active state.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public bool CCD
        {
            get
            {
                Get_InternalCCD(NativePtr, out var data);
                return data;
            }
            set => Set_InternalCCD(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets the dynamic state. True when this actor is dynamic (i.e. derived from DynamicRigidBodyActor class).
        /// </summary>
        public bool IsDynamic
        {
            get
            {
                Get_InternalIsDynamic(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the sleeping state. Returns true when this actor is sleeping or inactive.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public bool IsSleeping
        {
            get
            {
                Get_InternalIsSleeping(NativePtr, out var data);
                return data;
            }
            set => Set_InternalIsSleeping(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets the kinematic state. Returns true when this actor is kinematic or inactive.
        /// </summary>
        /// <remarks>Can be used only on dynamic RigidBody.</remarks>
        public bool IsKinematic
        {
            get
            {
                Get_InternalIsKinematic(NativePtr, out var data);
                return data;
            }
            set => Set_InternalIsKinematic(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAttachCollider(System.IntPtr nativePtr, System.IntPtr collider, bool awake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDetachCollider(System.IntPtr nativePtr, System.IntPtr collider, bool awakeOnLostTouch);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDetachColliders(System.IntPtr nativePtr, bool awakeOnLostTouch);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddForce(System.IntPtr nativePtr, Vector3 force, ForceMode forceMode, bool awake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddTorque(System.IntPtr nativePtr, Vector3 torque, ForceMode forceMode, bool awake);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RigidBodyActor InternalCreate();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RigidBodyActor InternalCreateDynamic();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern RigidBodyActor InternalCreateStatic();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalScene(System.IntPtr nativePtr, out PhysicsScene data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalColliders(System.IntPtr nativePtr, out Collider[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalSyncMode(System.IntPtr nativePtr, out RigidBodySyncMode data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalSyncMode(System.IntPtr nativePtr, ref RigidBodySyncMode data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalPosition(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalPosition(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalRotation(System.IntPtr nativePtr, out Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalRotation(System.IntPtr nativePtr, ref Quaternion data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAffectedByGravity(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalAffectedByGravity(System.IntPtr nativePtr, ref bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCollisionLayer(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalCollisionLayer(System.IntPtr nativePtr, ref uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVelocity(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalVelocity(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAngularVelocity(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalAngularVelocity(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCentreOfMass(System.IntPtr nativePtr, out Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalCentreOfMass(System.IntPtr nativePtr, ref Vector3 data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMaxAngularVelocity(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalMaxAngularVelocity(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalLinearDamping(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalLinearDamping(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAngularDamping(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalAngularDamping(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMass(System.IntPtr nativePtr, out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalMass(System.IntPtr nativePtr, ref float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCCD(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalCCD(System.IntPtr nativePtr, ref bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsDynamic(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsSleeping(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalIsSleeping(System.IntPtr nativePtr, ref bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsKinematic(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalIsKinematic(System.IntPtr nativePtr, ref bool data);
    }
}