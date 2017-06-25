// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.IO;

namespace ReCrafted.APIGen
{
    internal static class Entry
    {
        public static void Main()
        {
            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("tools"))
                currentDir += "\\..";

            var apiSourceFiles = Directory.GetFiles(currentDir + "\\src\\ReCrafted\\", "*.API.cpp", SearchOption.AllDirectories);

            var apibuilder = new APIBuilder();

            foreach (var sourceFile in apiSourceFiles)
            {
                Console.WriteLine("Processing: " + Path.GetFileName(sourceFile));

                string targetFile;
                var code = apibuilder.GenerateCode(sourceFile, out targetFile);

                File.WriteAllText("Test.cs", code);
            }

            Console.WriteLine("Generated");
            Console.ReadLine();
        }
    }
}