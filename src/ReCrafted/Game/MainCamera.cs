// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Common;
using ReCrafted.API.Graphics;

namespace ReCrafted.Game
{
    public class MainCamera : Script
    {
        protected internal override void OnCreate()
        {
            Camera = Camera.Create();
            Camera.SetAsCurrent();
        }

        protected override void OnUpdate()
        {

            //Camera.Position = Entity.Position;
        }

        public Camera Camera { get; private set; }

        public static MainCamera Current { get; set; }
    }
}
