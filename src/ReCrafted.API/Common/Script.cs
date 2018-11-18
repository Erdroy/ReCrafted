// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common.Actors;

namespace ReCrafted.API.Common
{
    public partial class Script
    {
        public TActor GetActor<TActor>() where TActor : ActorBase
        {
            return (TActor)Actor;
        }
    }
}
