// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.API.Core.Actors;

namespace ReCrafted.API
{
    public partial class Script
    {
        public TActor GetActor<TActor>() where TActor : ActorBase
        {
            return (TActor)InternalActor(NativePtr);
        }
    }

    public class Script<TActor> : Script where TActor : ActorBase
    {
        public TActor Actor => GetActor<TActor>();
    }
}
