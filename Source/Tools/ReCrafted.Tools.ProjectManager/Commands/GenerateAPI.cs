﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

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
            if (Options.Current.SkipAPI)
                return;

            const string c99CppHeaderExtension = ".h";
            const string c99CppHeaderAPIExtension = ".Gen.cpp";

            var timer = new Stopwatch();
            timer.Start();

            Console.WriteLine("API generation start");

            Console.WriteLine("Looking for API-defining headers");
            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("Tools"))
                currentDir += "\\..";

            var engineProjDir = currentDir + "\\Source\\Engine\\ReCrafted.Engine\\";

            var headerFiles = Directory.GetFiles(engineProjDir, "*.h", SearchOption.AllDirectories);

            var apiHeaderFiles = new List<string>();

            foreach (var headerFile in headerFiles)
            {
                var sourceCode = File.ReadAllText(headerFile);
                if (sourceCode.Contains("API_CLASS") || sourceCode.Contains("API_STRUCT"))
                    apiHeaderFiles.Add(headerFile);
            }

            Console.WriteLine($"Found {apiHeaderFiles.Count} API-defining headers out of total {headerFiles.Length} ({timer.ElapsedMilliseconds} ms)");
            Console.WriteLine("Generating API files...");

            var classList = new ClassList();

            foreach (var headerFile in apiHeaderFiles)
            {
                var fileInfo = new FileInfo(headerFile);

                if (fileInfo.Name == "ReCraftedAPI.h")
                    continue;

                var headerFileName = Path.GetFileNameWithoutExtension(fileInfo.Name);
                var apiSourceFileName = Path.GetFileNameWithoutExtension(fileInfo.Name) + ".Gen.cs";

                if (!headerFile.EndsWith(c99CppHeaderExtension) || fileInfo.Extension != c99CppHeaderExtension)
                    throw new Exception($"API-defining header file '{headerFile}' does not end with C/C++ header file extension ({c99CppHeaderExtension}).");

                //try
                {
                    // Construct C++ file for API output.
                    // Replace .h with .Gen.cpp.
                    var proxySourceFile = headerFile.Remove(headerFile.Length - c99CppHeaderExtension.Length, c99CppHeaderExtension.Length);
                    proxySourceFile += c99CppHeaderAPIExtension;

                    classList.Includes.Add(headerFile.Replace(engineProjDir, "").Replace("\\", "/"));
                    classList.Names.Add(headerFileName);

                    if (File.Exists(proxySourceFile))
                    {
                        var apiFileInfo = new FileInfo(proxySourceFile);

                        if (apiFileInfo.LastWriteTime > fileInfo.LastWriteTime)
                        {
                            Console.WriteLine($"API for '{headerFileName}' will not be generated. No changes found.");
                            continue;
                        }
                    }

                    // Generate C# and C++ class from given header file
                    var generator = new Generator(headerFile);
                    generator.Parse();

                    // Construct C# file for API output.
                    var nameSpace = generator.Namespace.Replace("ReCrafted.API", "");
                    var apiSourceFile =
                        $"{currentDir}\\Source\\Engine\\ReCrafted.API{nameSpace.Replace(".", "\\")}\\{apiSourceFileName}";

                    // Create directory if missing
                    var apiSourceFileDirectory = apiSourceFile.Replace(apiSourceFileName, "");

                    if (!Directory.Exists(apiSourceFileDirectory))
                        Directory.CreateDirectory(apiSourceFileDirectory);

                    generator.Generate(apiSourceFile, proxySourceFile);
                }
                //catch (Exception ex)
                {
                    //Console.WriteLine($"Failed to generate API file using header file '{headerFile}'.\nError: \n{ex.Message}");
                }
            }

            Console.WriteLine("Generating runtime initialization...");
            Generator.GenerateRuntimeInitialization(classList, engineProjDir + "Scripting\\ScriptingManager.Gen.cpp");

            timer.Stop();
            Console.WriteLine($"API Generated in {timer.ElapsedMilliseconds} ms.");
        }
    }
}
