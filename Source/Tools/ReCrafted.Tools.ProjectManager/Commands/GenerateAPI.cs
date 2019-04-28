// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using ReCrafted.Tools.APIGenerator;

namespace ReCrafted.Tools.ProjectManager.Commands
{
    public class GenerateAPI : Command
    {
        public override void Execute()
        {
            const string c99CppHeaderExtension = ".h";

            var timer = new Stopwatch();
            timer.Start();

            Console.WriteLine("API generation start");

            Console.WriteLine("Looking for API-defining headers");
            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("Tools"))
                currentDir += "\\..";

            var headerFiles = Directory.GetFiles(currentDir + "\\Source\\Engine\\ReCrafted.Engine\\", "*.h", SearchOption.AllDirectories);

            var apiHeaderFiles = new List<string>();

            foreach (var headerFile in headerFiles)
            {
                var sourceCode = File.ReadAllText(headerFile);
                if (sourceCode.Contains("API_CLASS") || sourceCode.Contains("API_STRUCT"))
                    apiHeaderFiles.Add(headerFile);
            }

            Console.WriteLine($"Found {apiHeaderFiles.Count} API-defining headers out of total {headerFiles.Length} ({timer.ElapsedMilliseconds} ms)");

            // Set generator options
            APIGenerator.Options.Current = new APIGenerator.Options
            {
                Verbose = true
            };

            Console.WriteLine("Generating API files...");
            foreach (var headerFile in apiHeaderFiles)
            {
                var fileInfo = new FileInfo(headerFile);
                var apiSourceFileName = Path.GetFileNameWithoutExtension(fileInfo.Name) + ".Gen.cs";

                if (!headerFile.EndsWith(c99CppHeaderExtension) || fileInfo.Extension != c99CppHeaderExtension)
                    throw new Exception($"API-defining header file '{headerFile}' does not end with C/C++ header file extension ({c99CppHeaderExtension}).");

                // Construct C++ file for API output.
                // Replace .h with .Gen.cpp.
                var proxySourceFile = headerFile.Remove(headerFile.Length - c99CppHeaderExtension.Length, c99CppHeaderExtension.Length);
                proxySourceFile += ".Gen.cpp";

                // Construct C# file for API output.
                var nameSpace = Generator.ParseNamespace(headerFile).Replace("ReCrafted.API.", "");
                var apiSourceFile = currentDir + "\\Source\\Engine\\ReCrafted.API\\" + nameSpace.Replace(".", "\\") + "\\";
                apiSourceFile += apiSourceFileName;

                // Create directory if missing
                var apiSourceFileDirectory = apiSourceFile.Replace(apiSourceFileName, "");
                if (!Directory.Exists(apiSourceFileDirectory))
                    Directory.CreateDirectory(apiSourceFileDirectory);

                // Generate C# and C++ class from given header file
                var generator = new Generator(headerFile, apiSourceFile, proxySourceFile);
                generator.GenerateClass();
            }

            timer.Stop();
            Console.WriteLine($"API Generated in {timer.ElapsedMilliseconds} ms.");
        }
    }
}
