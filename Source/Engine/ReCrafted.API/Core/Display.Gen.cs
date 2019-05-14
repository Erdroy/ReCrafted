// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
    /// <summary>
    ///     The display class.
    /// </summary>
    public static partial class Display
    {

        /// <summary>
        ///     Gets the aspect ratio of the screen.
        /// </summary>
        public static float AspectRatio
        {
            get
            {
                Get_InternalAspectRatio(out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the screen width.
        /// </summary>
        public static uint Width
        {
            get
            {
                Get_InternalWidth(out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the screen height.
        /// </summary>
        public static uint Height
        {
            get
            {
                Get_InternalHeight(out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalAspectRatio(out float data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalWidth(out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHeight(out uint data);
    }
}