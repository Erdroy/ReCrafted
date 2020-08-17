// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.WebUI 
{
    /// <summary>
    ///     WebUIManager class. Provides WebUIView management, UI engine initialization and shutdown and more.
    /// </summary>
    internal static partial class WebUIManager
    {
        
        /// <summary>
        /// Creates new <see cref="WebUIView"/> of given width and height.
        /// </summary>
        /// <param name="width">The width.</param>
        /// <param name="height">The height.</param>
        /// <returns>The created WebUIView.</returns>
        public static WebUIView CreateView(int width, int height)
        {
            return InternalCreateView(width, height);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern WebUIView InternalCreateView(int width, int height);
    }
}