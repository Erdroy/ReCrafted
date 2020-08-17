// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
    /// <summary>
    ///     Logger class. Provides logging functionality.
    /// </summary>
    public static partial class Logger
    {

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalWriteLog(LogLevel level, string str);
    }
}