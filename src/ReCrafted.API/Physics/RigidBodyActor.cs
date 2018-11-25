// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Physics
{
    public partial class RigidBodyActor
    {
        /// <summary>
        /// Creates new actor of type 'RigidBodyActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
        public static RigidBodyActor Create()
        {
            return New<RigidBodyActor>();
        }
    }
}
