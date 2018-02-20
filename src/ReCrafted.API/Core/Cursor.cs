// ReCrafted (c) 2016-2018 Always Too Late

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
            get;
            set;
            //set { Game.LockCursor = value; }
            //get { return Game.LockCursor; }
        }

        /// <summary>
        /// Gets or sets cursor state.
        /// </summary>
        public static bool Show
        {
            get;
            set;
            //set { Game.ShowCursor = value; }
            //get { return Game.ShowCursor; }
        }

        /// <summary>
        /// Gets or sets cursor icon.
        /// </summary>
        public static CursorIcon Icon
        {
            get;
            set;
            //set { Game.CursorIcon = (int)value; }
            //get { return (CursorIcon)Game.CursorIcon; }
        }
    }
}
