// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ProjectManager
{
    /// <inheritdoc />
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
        /// The username of the author who created this build.
        /// </summary>
        public string BuildAuthor { get; set; }

        /// <summary>
        /// The build date.
        /// </summary>
        public DateTime BuildTime { get; set; }

        /// <summary>
        /// The current GameInfo, contains all values of the current build.
        /// </summary>
        public static GameInfo Current { get; set; }
    }
}
