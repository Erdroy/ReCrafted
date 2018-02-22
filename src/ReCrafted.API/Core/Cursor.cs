// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Common;

namespace ReCrafted.API.Core
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
            set => Input.LockCursor = value;
            get => Input.LockCursor;
        }

        /// <summary>
        /// Gets or sets cursor state.
        /// </summary>
        public static bool Show
        {
            set => Input.ShowCursor = value;
            get => Input.ShowCursor;
        }

        /// <summary>
        /// Gets or sets cursor icon.
        /// </summary>
        public static CursorIcon Icon
        {
            set { Application.CursorIcon = (int)value; }
            get { return (CursorIcon)Application.CursorIcon; }
        }
    }
}
