// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.IO;

namespace ReCrafted.APIGen
{
    internal static class Entry
    {
        public static void Main()
        {
            var apiSourceFiles = Directory.GetFiles(Environment.CurrentDirectory + "/src/ReCrafted/", "*.API.cpp", SearchOption.AllDirectories);

            foreach (var file in apiSourceFiles)
            {
                var fileName = file.Replace("/", "\\");

                var objects = Parser.ParseFile(fileName);

                foreach (var src in objects)
                {
                    
                }

                // TODO: generate API for every object
            }
        }
    }
}