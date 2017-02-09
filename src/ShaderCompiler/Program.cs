
using System;
using System.Diagnostics;
using System.IO;

namespace ShaderCompiler
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Console.WriteLine("Starting compiling shaders...");

            // tools\\shaderc.exe -f src/ReCrafted/Shaders/testShader/vs_X.sc -o ???/vs_X.bin --platform windows --type v
            // tools\\shaderc.exe -f src/ReCrafted/Shaders/testShader/fs_X.sc -o ???/fs_X.bin --platform windows --type f
            foreach (var shader in Directory.GetDirectories(args[0] + "src\\ReCrafted\\Shaders"))
            {
                var fileName = new FileInfo(shader);

                Console.WriteLine("Compiling: " + fileName.Name);

                var outDir = args[0] + "assets\\shaders\\dx11\\" + fileName.Name + "\\";
                if (!Directory.Exists(outDir))
                {
                    Directory.CreateDirectory(outDir);
                    Console.WriteLine("Created shader directory: " + outDir);
                }
                var prc = Process.Start(args[0] + "tools\\shaderc.exe", 
                    "-f " + shader + "/vs_" + fileName.Name + ".sc " +
                    "-o " + outDir + "/vs_" + fileName.Name + ".bin " +
                    "--platform windows " +
                    "--verbose " +
                    "--type v");

                prc.WaitForExit();

                prc = Process.Start(args[0] + "tools\\shaderc.exe",
                    "-f " + shader + "/fs_" + fileName.Name + ".sc " +
                    "-o " + outDir + "/fs_" + fileName.Name + ".bin " +
                    "--platform windows " +
                    "--verbose " +
                    "--type f");

                prc.WaitForExit();
            }
        }
    }
}