// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.APIBuilder
{
    internal static class Entry
    {
        public static void Main()
        {
            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("tools"))
                currentDir += "\\..";

            var apiSourceFiles = Directory.GetFiles(currentDir + "\\src\\ReCrafted.Engine\\", "*.API.cpp", SearchOption.AllDirectories);

            var apibuilder = new APIBuilder();

            foreach (var sourceFile in apiSourceFiles)
            {
                Console.WriteLine("Processing: " + Path.GetFileName(sourceFile));

                apibuilder.Build(sourceFile, currentDir);
            }

            Console.WriteLine("Generated");
        }
    }
}