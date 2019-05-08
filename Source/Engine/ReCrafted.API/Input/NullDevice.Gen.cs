// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
    /// <summary>
    ///     Null input device.
    /// </summary>
    public sealed class NullDevice : InputDevice
    {
        
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
        private static extern string InternalDeviceName(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceType InternalType(System.IntPtr nativePtr);
    }
}