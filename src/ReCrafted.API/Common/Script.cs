// ReCrafted (c) 2016-2018 Always Too Late

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
