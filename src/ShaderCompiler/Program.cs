
using System;
using System.Diagnostics;
using System.IO;

namespace ShaderCompiler
{
    internal class Program
    {
        private static void BuildShader(string path, string shader, string outDir, string shaderName, string profile, string type, string prefix)
        {
            Console.WriteLine("Compiling: " + prefix + "_" + shaderName);
            var proc = new Process
            {
                StartInfo = new ProcessStartInfo(path + "tools\\shaderc.exe",
                    " -f " + shader + "/" + prefix + "_" + shaderName + ".glsl " +
                    " -o " + outDir + "/" + prefix + "_" + shaderName + ".bin " +
                    " --platform " + profile +
                    " --type " + type)
                {
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true
                }
            };

            // for debug
            // proc.StartInfo.Arguments += " --disasm";

            proc.Start();
            proc.WaitForExit();

            var error = proc.StandardError.ReadToEnd();
            if (error.Length > 1)
            {
                Console.WriteLine(error);
                Console.WriteLine("ERROR: Shader compilation failed!!!");
                Environment.Exit(-1);
            }
        }

        private static void Main(string[] args)
        {
            Console.WriteLine("Starting compiling shaders...");
            
            foreach (var shader in Directory.GetDirectories(args[0] + "src\\ReCrafted\\Shaders"))
            {
                var fileName = new FileInfo(shader);

                var outDir = args[0] + "assets\\shaders\\dx11\\" + fileName.Name + "\\";
                if (!Directory.Exists(outDir))
                    Directory.CreateDirectory(outDir);

                BuildShader(args[0], shader, outDir, fileName.Name, "windows -p vs_5_0 -O 3", "vertex", "vs");
                BuildShader(args[0], shader, outDir, fileName.Name, "windows -p ps_5_0 -O 3", "fragment", "fs");
            }

            Console.WriteLine("All shaders compiled!");
        }
    }
}