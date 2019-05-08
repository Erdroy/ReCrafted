// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
    /// <summary>
    ///     Mouse input device.
    /// </summary>
    public sealed class Mouse : InputDevice
    {
        
        /// <summary>
        ///     Emits given button and it's state.
        /// </summary>
        /// <param name="button">The button.</param>
        /// <param name="buttonState">The button state.</param>
        public void EmitInput(Button button, ButtonState buttonState)
        {
            InternalEmitInput(NativePtr, button, buttonState);
        }
        
        /// <summary>
        ///     Emits cursor move.
        /// </summary>
        /// <param name="position">The new position.</param>
        /// <param name="delta">The cursor delta.</param>
        public void EmitCursor(Vector2 position, Vector2 delta)
        {
            InternalEmitCursor(NativePtr, position, delta);
        }
        
        /// <summary>
        ///     Emits scroll move.
        /// </summary>
        /// <param name="delta">The scroll delta.</param>
        public void EmitScroll(float delta)
        {
            InternalEmitScroll(NativePtr, delta);
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
        
        /// <summary>
        ///     Returns true when specified button is being held for at least one frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public bool IsButton(Button button)
        {
            return InternalIsButton(NativePtr, button);
        }
        
        /// <summary>
        ///     Returns true when specified button is has been pressed this frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public bool IsButtonDown(Button button)
        {
            return InternalIsButtonDown(NativePtr, button);
        }
        
        /// <summary>
        ///     Returns true when specified button is has been released this frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public bool IsButtonUp(Button button)
        {
            return InternalIsButtonUp(NativePtr, button);
        }
        
        /// <summary>
        ///     Sets mouse position relative to the current game window.
        /// </summary>
        /// <param name="position">The new position.</param>
        public void SetCursorPosition(Vector2 position)
        {
            InternalSetCursorPosition(NativePtr, position);
        }
        
        /// <summary>
        ///     Gets mouse position relative to the current game window.
        /// </summary>
        public Vector2 GetCursorPosition()
        {
            return InternalGetCursorPosition(NativePtr);
        }
        
        /// <summary>
        ///     Gets mouse position delta from last frame.
        /// </summary>
        public Vector2 GetCursorDelta()
        {
            return InternalGetCursorDelta(NativePtr);
        }
        
        /// <summary>
        ///     Gets mouse scroll delta from last frame.
        /// </summary>
        public float GetScrollDelta()
        {
            return InternalGetScrollDelta(NativePtr);
        }
        
        /// <summary>
        ///     Sets cursor show state.
        /// </summary>
        public void SetShowCursor(bool cursorShow)
        {
            InternalSetShowCursor(NativePtr, cursorShow);
        }
        
        /// <summary>
        ///     Gets cursor show state.
        /// </summary>
        public bool GetShowCursor()
        {
            return InternalGetShowCursor(NativePtr);
        }
        
        /// <summary>
        ///     Sets cursor lock state.
        /// </summary>
        public void SetLockCursor(bool cursorLock)
        {
            InternalSetLockCursor(NativePtr, cursorLock);
        }
        
        /// <summary>
        ///     Gets cursor lock state.
        /// </summary>
        public bool GetLockCursor()
        {
            return InternalGetLockCursor(NativePtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEmitInput(System.IntPtr nativePtr, Button button, ButtonState buttonState);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEmitCursor(System.IntPtr nativePtr, Vector2 position, Vector2 delta);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEmitScroll(System.IntPtr nativePtr, float delta);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string InternalDeviceName(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceType InternalType(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButton(System.IntPtr nativePtr, Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButtonDown(System.IntPtr nativePtr, Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButtonUp(System.IntPtr nativePtr, Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetCursorPosition(System.IntPtr nativePtr, Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 InternalGetCursorPosition(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 InternalGetCursorDelta(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalGetScrollDelta(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetShowCursor(System.IntPtr nativePtr, bool cursorShow);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalGetShowCursor(System.IntPtr nativePtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLockCursor(System.IntPtr nativePtr, bool cursorLock);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalGetLockCursor(System.IntPtr nativePtr);
    }
}