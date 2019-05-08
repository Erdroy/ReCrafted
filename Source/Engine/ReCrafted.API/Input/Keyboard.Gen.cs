// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
    /// <summary>
    ///     Keyboard input device.
    /// </summary>
    public sealed class Keyboard : InputDevice
    {
        
        /// <summary>
        ///     Emits given key and it's state.
        /// </summary>
        /// <param name="key">The key.</param>
        /// <param name="keyState">The key state.</param>
        public void EmitInput(Key key, KeyState keyState)
        {
            InternalEmitInput(NativePtr, key, keyState);
        }
        
        /// <summary>
        ///     Emits given character.
        /// </summary>
        /// <param name="character">The character.</param>
        public void EmitCharacter(char character)
        {
            InternalEmitCharacter(NativePtr, character);
        }
        
        /// <summary>
        ///     The name of this device.
        /// </summary>
        public override string DeviceName()
        {
            return InternalDeviceName(NativePtr);
        }
        
        /// <summary>
        ///     The type of this device.
        /// </summary>
        public override DeviceType Type()
        {
            return InternalType(NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEmitInput(System.IntPtr nativePtr, Key key, KeyState keyState);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEmitCharacter(System.IntPtr nativePtr, char character);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string InternalDeviceName(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceType InternalType(System.IntPtr nativePtr);
    }
}