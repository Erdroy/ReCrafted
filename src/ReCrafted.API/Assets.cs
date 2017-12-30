// ReCrafted © 2016-2017 Always Too Late

using System;

namespace ReCrafted.API
{
    /// <summary>
    /// Type of asset.
    /// </summary>
    public enum AssetType
    {
        /// <summary>
        /// Textures asset directory.
        /// </summary>
        Texture,

        /// <summary>
        /// Fonts asset directory.
        /// </summary>
        Font,

        /// <summary>
        /// Interface textures asset directory.
        /// </summary>
        Interface,

        /// <summary>
        /// Default asset dirextory.
        /// </summary>
        None
    }

    public static class Assets
    {
        /// <summary>
        /// Path to assets folder.
        /// </summary>
        public const string AssetPath = "../assets/";

        /// <summary>
        /// Path to textures folder.
        /// </summary>
        public const string TexturePath = AssetPath + "textures/";

        /// <summary>
        /// Path to interface textures folder.
        /// </summary>
        public const string InterfacePath = TexturePath + "interface/";

        /// <summary>
        /// Path to font folder.
        /// </summary>
        public const string FontPath = AssetPath + "fonts/";

        /// <summary>
        /// Returns file path from default asset directory.
        /// </summary>
        /// <param name="name">File name with extension.</param>
        /// <returns></returns>
        public static string ResolveAssetFilePath(string name)
        {
            return ResolveAssetFilePath(AssetType.None, name);
        }

        /// <summary>
        /// Returns file path for given resource type.
        /// </summary>
        /// <param name="type">Our resource type.</param>
        /// <param name="name">File name with extension.</param>
        /// <returns></returns>
        public static string ResolveAssetFilePath(AssetType type, string name)
        {
            switch (type)
            {
                case AssetType.None:
                    return $"{AssetPath}{name}";
                case AssetType.Texture:
                    return $"{TexturePath}{name}";
                case AssetType.Font:
                    return $"{FontPath}{name}";
                case AssetType.Interface:
                    return $"{InterfacePath}{name}";
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }
        }
    }
}
