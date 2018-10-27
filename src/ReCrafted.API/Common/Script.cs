// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.CompilerServices;
using ReCrafted.API.Common.Components;

namespace ReCrafted.API.Common
{
    public partial class Script
    {
        /// <summary>
        /// Contains the reference to transform owned by this game object.
        /// </summary>
        public ref TransformComponent Transform => ref GameObject.Transform;
    }
}
