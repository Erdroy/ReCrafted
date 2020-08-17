// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

namespace ReCrafted.API.Input
{
    public abstract class InputDevice : Object
    {
        /// <summary>
        ///     The name of this device.
        /// </summary>
        public abstract string DeviceName();

        /// <summary>
        ///     The type of this device.
        /// </summary>
        public abstract DeviceType Type();
    }
}
