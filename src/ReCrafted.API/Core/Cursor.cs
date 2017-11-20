// ReCrafted © 2016-2017 Always Too Late

namespace ReCrafted.API.Core
{
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
            set { Game.LockCursor = value; }
            get { return Game.LockCursor; }
        }

        /// <summary>
        /// Gets or sets cursor state.
        /// </summary>
        public static bool Show
        {
            set { Game.ShowCursor = value; }
            get { return Game.ShowCursor; }
        }
    }
}
