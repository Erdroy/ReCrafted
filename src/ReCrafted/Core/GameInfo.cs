// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.Common;

namespace ReCrafted.Core
{
    /// <summary>
    /// GameInfo JSON class.
    /// </summary>
    public class GameInfo : JsonTemplate<GameInfo>
    {
        /// <summary>
        /// Default constructor.
        /// </summary>
        public GameInfo()
        {
            Current = this;
        }

        /// <summary>
        /// The current build name.
        /// </summary>
        public string BuildName { get; set; }

        /// <summary>
        /// The current build number.
        /// </summary>
        public int BuildNumber { get; set; }

        /// <summary>
        /// The current GameInfo, contains all values of the current build.
        /// </summary>
        public static GameInfo Current { get; set; }
    }
}
