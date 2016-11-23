
using System;
using ReCrafted.Core;

namespace ReCrafted.VersionBuilder
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Console.WriteLine("Build num file: " + Environment.CurrentDirectory + "\\" + args[1]);
            Console.WriteLine("Output file: " + Environment.CurrentDirectory + "\\" + args[2]);
            GameInfo.Generate(args[0], Environment.CurrentDirectory + "\\" +args[1], Environment.CurrentDirectory + "\\" + args[2]);
        }
    }
}