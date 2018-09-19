// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.IO;

namespace ReCrafted.ProjectManager.Commands
{
    using Compiler = ShaderCompiler.ShaderCompiler;

    public class CompileShaders : Command
    {
        public override void Execute()
        {
            Console.WriteLine("Compiling shaders...");

            var shadersDirectory = Path.Combine(Program.RootDirectory, "src\\ReCrafted.Engine\\Shaders\\");

            // make sure that we are in the correct directory
            if (!Directory.Exists(shadersDirectory))
            {
                Console.WriteLine("Shaders directory is missing! " +
                                  "Make sure, that working directory is the root directory of the project.");
                Environment.Exit(-1);
            }

            // find all *.hlsl files that we want to compile
            var shaders = Directory.GetFiles(shadersDirectory, "*.hlsl", SearchOption.AllDirectories);
            
            // setup fake options
            ShaderCompiler.Options.Current = new ShaderCompiler.Options
            {
                CuteJson = true,
                DisassembleOutput = false,
                Verbose = false
            };

            // compile all selected files to relative directory in game: .\
            foreach (var shaderFile in shaders)
            {
                var shaderFileName = shaderFile.Remove(0, shadersDirectory.Length);
                var shaderFileOutput = Path.Combine(Program.RootDirectory, "build\\assets\\shaders", Path.GetFileNameWithoutExtension(shaderFileName) + ".shader");
                var path = Path.GetDirectoryName(shaderFileOutput);

                if (!Directory.Exists(path))
                    Directory.CreateDirectory(path ?? throw new InvalidOperationException());
                
                Console.WriteLine($"\nCompiling shader file '{shaderFileName}'");

                if (!Compiler.CompileFile(shaderFile, shaderFileOutput))
                {
                    Environment.Exit(-1);
                }
            }

            // done!
        }
    }
}
