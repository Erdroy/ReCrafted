// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
    /// <summary>
    ///     InputManager class. Provides user input functionality.
    /// </summary>
    public static partial class InputManager
    {
        
        /// <summary>
        ///     Returns true when specified button is being held for at least one frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public static bool IsButton(Button button)
        {
            return InternalIsButton(button);
        }
        
        /// <summary>
        ///    Returns true when specified button is has been pressed this frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public static bool IsButtonDown(Button button)
        {
            return InternalIsButtonDown(button);
        }
        
        /// <summary>
        ///    Returns true when specified button is has been released this frame.
        /// </summary>
        /// <param name="button">The button.</param>
        public static bool IsButtonUp(Button button)
        {
            return InternalIsButtonUp(button);
        }
        
        /// <summary>
        ///     Sets mouse position relative to the current game window.
        /// </summary>
        /// <param name="position">The new position.</param>
        public static void SetCursorPosition(Vector2 position)
        {
            InternalSetCursorPosition(position);
        }
        
        /// <summary>
        ///     Gets mouse position relative to the current game window.
        /// </summary>
        public static Vector2 GetCursorPosition()
        {
            return InternalGetCursorPosition();
        }
        
        /// <summary>
        ///     Gets mouse position delta from last frame.
        /// </summary>
        public static Vector2 GetCursorDelta()
        {
            return InternalGetCursorDelta();
        }
        
        /// <summary>
        ///     Gets mouse scroll delta from last frame.
        /// </summary>
        public static float GetScrollDelta()
        {
            return InternalGetScrollDelta();
        }
        
        /// <summary>
        ///     Sets cursor show state.
        /// </summary>
        /// <param name="showCursor">The show state.</param>
        public static void SetShowCursor(bool showCursor)
        {
            InternalSetShowCursor(showCursor);
        }
        
        /// <summary>
        ///     Gets cursor show state.
        /// </summary>
        public static bool GetShowCursor()
        {
            return InternalGetShowCursor();
        }
        
        /// <summary>
        ///     Sets cursor lock state.
        /// </summary>
        /// <param name="lockCursor">The lock state.</param>
        public static void SetLockCursor(bool lockCursor)
        {
            InternalSetLockCursor(lockCursor);
        }
        
        /// <summary>
        ///     Gets cursor lock state.
        /// </summary>
        public static bool GetLockCursor()
        {
            return InternalGetLockCursor();
        }
        
        /// <summary>
        ///     Returns true when specified key is being held for at least one frame.
        /// </summary>
        /// <param name="key">The key.</param>
        public static bool IsKey(Key key)
        {
            return InternalIsKey(key);
        }
        
        /// <summary>
        ///     Returns true when specified key is has been pressed this frame.
        /// </summary>
        /// <param name="key">The key.</param>
        public static bool IsKeyDown(Key key)
        {
            return InternalIsKeyDown(key);
        }
        
        /// <summary>
        ///     Returns true when specified key is has been released this frame.
        /// </summary>
        /// <param name="key">The key.</param>
        public static bool IsKeyUp(Key key)
        {
            return InternalIsKeyUp(key);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButton(Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButtonDown(Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsButtonUp(Button button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetCursorPosition(Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 InternalGetCursorPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector2 InternalGetCursorDelta();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern float InternalGetScrollDelta();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetShowCursor(bool showCursor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalGetShowCursor();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLockCursor(bool lockCursor);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalGetLockCursor();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsKey(Key key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsKeyDown(Key key);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsKeyUp(Key key);
    }
}