// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core.Actors 
{
    /// <summary>
    ///     EmptyActor - provides an actor that has no additional functionality.
    /// </summary>
    public sealed class EmptyActor : ActorBase
    {
        
        /// <summary>
        ///     Creates new actor of type <see cref="EmptyActor"/>.
        /// </summary>
        public static EmptyActor Create()
        {
            return InternalCreate();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern EmptyActor InternalCreate();
    }
}