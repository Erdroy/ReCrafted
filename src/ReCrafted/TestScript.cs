// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Core;

namespace ReCrafted.Game
{
    public class TestScript : Script
    {
        protected internal override void OnCreate()
        {
            Logger.Write(this + " - Hello, World! Entity: " + Entity.Name);
        }
    }
}
