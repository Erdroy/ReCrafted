// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Core;

namespace ReCrafted
{
    internal static class Program
    {
        [MTAThread]
        private static void Main()
        {
            using (var game = new Game())
            {
                game.Run();
            }
        }
    }
}