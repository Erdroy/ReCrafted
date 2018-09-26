// ReCrafted Editor (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.Editor.Utilities
{
    public class PathUtils
    {
        public static string NormalizePath(string path)
        {
            return Path.GetFullPath(new Uri(path).LocalPath)
                .TrimEnd(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);
        }
    }
}
