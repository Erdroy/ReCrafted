// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
    /// <summary>
    ///     Application class. Provides application management functionality.
    /// </summary>
    public static class Application
    {
        
        /// <summary>
        ///     Quits the current application.
        /// </summary>
        public static void Quit()
        {
            InternalQuit();
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalQuit();
    }
}