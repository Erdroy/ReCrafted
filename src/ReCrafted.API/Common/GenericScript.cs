// ReCrafted (c) 2016-2019 Always Too Late

using ReCrafted.API.Common.Actors;

namespace ReCrafted.API.Common
{
    /// <summary>
    /// GenericScript class. Implements custom Actor type for Script class.
    /// </summary>
    /// <typeparam name="TActor"></typeparam>
    public class GenericScript<TActor> : Script where TActor : ActorBase
    {
        /// <summary>
        /// The Actor instance of type <see cref="TActor"/> that contains this script.
        /// </summary>
        public new TActor Actor => (TActor)base.Actor;
    }
}
