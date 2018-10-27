// ReCrafted (c) 2016-2018 Always Too Late

using System.Runtime.CompilerServices;
using ReCrafted.API.Common.Components;

namespace ReCrafted.API.Common
{
    public partial class GameObject
    {
        private GameObject() { }
        
        public TScript AddScript<TScript>() where TScript : Script, new()
        {
            var script = (TScript)NewGeneric<Script>(new TScript());

            Internal_AddScript(NativePtr, script.NativePtr);

            return script;
        }

        public void RemoveScript<TScript>(TScript script) where TScript : Script
        {
            Internal_RemoveScript(NativePtr, script.NativePtr);
        }

        /// <summary>
        /// Contains the reference to transform owned by this game object.
        /// </summary>
        public unsafe ref TransformComponent Transform => ref Unsafe.AsRef<TransformComponent>(Internal_GetTransform(NativePtr));
    }
}
