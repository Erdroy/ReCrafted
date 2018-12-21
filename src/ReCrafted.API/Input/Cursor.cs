// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Core;

namespace ReCrafted.API.Input
{
    /// <summary>
    /// Type of cursor icon.
    /// </summary>
    public enum CursorIcon
    {
        /// <summary>
        /// Icon of cursor is unknown.
        /// </summary>
        Unknown = 999,

        /// <summary>
        /// Standard mouse icon.
        /// </summary>
        Arrow = 0,

        /// <summary>
        /// IBeam icon.
        /// </summary>
        Beam = 1
    }

    /// <summary>
    /// Cursor class. Manages cursor state and lock mode.
    /// </summary>
    public static class Cursor
    {
        /// <summary>
        /// Gets or sets cursor lock mode.
        /// </summary>
        public static bool Lock
        {
            set => InputManager.LockCursor = value;
            get => InputManager.LockCursor;
        }

        /// <summary>
        /// Gets or sets cursor state.
        /// </summary>
        public static bool Show
        {
            set => InputManager.ShowCursor = value;
            get => InputManager.ShowCursor;
        }

        /// <summary>
        /// Gets or sets cursor icon.
        /// </summary>
        public static CursorIcon Icon
        {
            set => Application.CursorIcon = (int)value;
            get => (CursorIcon)Application.CursorIcon;
        }
    }
}
