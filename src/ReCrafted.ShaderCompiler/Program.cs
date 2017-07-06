using System;
using System.Diagnostics;
using System.IO;

namespace ReCrafted.ShaderCompiler
{
    class Program
    {
        private static void BuildShader(string path, string shader, string outDir, string shaderName, string profile, string type, string prefix)
        {
            Console.WriteLine(string.Concat("Compiling: ", prefix, "_", shaderName));
            Process process = new Process()
            {
                StartInfo = new ProcessStartInfo(string.Concat(path, "tools\\shaderc.exe"), string.Concat(new string[] { " -f ", shader, "/", prefix, "_", shaderName, ".glsl  -o ", outDir, "/", prefix, "_", shaderName, ".bin  --platform ", profile, " --type ", type }))
                {
                    CreateNoWindow = true,
                    UseShellExecute = false,
                    RedirectStandardOutput = true,
                    RedirectStandardError = true
                }
            };
            Process process1 = process;
            ProcessStartInfo startInfo = process1.StartInfo;
            startInfo.Arguments = string.Concat(startInfo.Arguments, " --disasm");
            process1.Start();
            process1.WaitForExit();
            string end = process1.StandardError.ReadToEnd();
            if (end.Length > 1)
            {
                Console.WriteLine(end);
                Console.WriteLine("ERROR: Shader compilation failed!!!");
                Environment.Exit(-1);
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Starting compiling shaders...");
            string[] directories = Directory.GetDirectories(string.Concat(args[0], "src\\ReCrafted.Engine\\Shaders"));
            for (int i = 0; i < (int)directories.Length; i++)
            {
                string str = directories[i];
                FileInfo fileInfo = new FileInfo(str);
                string str1 = fileInfo.FullName.Remove(fileInfo.FullName.Length - fileInfo.Name.Length, fileInfo.Name.Length);
                string str2 = string.Concat(args[0], "build\\assets\\shaders\\dx11\\", fileInfo.Name, "\\");
                if (!Directory.Exists(str2))
                {
                    Directory.CreateDirectory(str2);
                }
                if (File.Exists(string.Concat(str1, fileInfo.Name, "\\desc.json")))
                {
                    File.Copy(string.Concat(str1, fileInfo.Name, "\\desc.json"), string.Concat(str2, "desc.json"), true);
                }
                Program.BuildShader(args[0], str, str2, fileInfo.Name, "windows -p vs_4_0 -O 3", "vertex", "vs");
                Program.BuildShader(args[0], str, str2, fileInfo.Name, "windows -p ps_4_0 -O 3", "fragment", "fs");
            }
            Console.WriteLine("All shaders compiled!");
        }
    }
}
