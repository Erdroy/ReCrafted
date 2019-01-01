// ReCrafted (c) 2016-2019 Always Too Late

namespace ReCrafted.API.Input
{
    public static partial class InputManager
    {
        /// <summary>
        /// Gets device by given deviceId.
        /// </summary>
        /// <typeparam name="TDevice">The target device type.</typeparam>
        /// <param name="deviceId">The device id.</param>
        /// <returns>The device.</returns>
        public static TDevice GetDevice<TDevice>(int deviceId) where TDevice : InputDevice
        {
            return (TDevice)GetDevice(deviceId);
        }

        /// <summary>
        /// Gets device by given device type (<see cref="DeviceType"/>).
        /// </summary>
        /// <typeparam name="TDevice">The target device type.</typeparam>
        /// <param name="deviceType">The device type.</param>
        /// <returns>The device.</returns>
        public static TDevice GetDevice<TDevice>(DeviceType deviceType) where TDevice : InputDevice
        {
            return (TDevice)GetDevice(deviceType);
        }
    }
}
