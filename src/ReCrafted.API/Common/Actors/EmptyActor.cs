// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Common.Actors
{
    public partial class EmptyActor
    {
        /// <summary>
        /// Creates new actor of type 'EmptyActor'.
        /// </summary>
        /// <returns>The newly created actor.</returns>
        public static EmptyActor Create()
        {
            return New<EmptyActor>();
        }
    }
}
