// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.IO;
using ReCrafted.Graphics.Renderers.D3D11;

namespace ShaderConverter
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            if (args.Length > 0)
            {
                var source = File.ReadAllText(args[0]);
                var parsedSource = D3D11ShaderCompiler.ParseShaderSource(source, args[0]);

                File.WriteAllText(args[0]+".hlslo", parsedSource);
            }
        }
    }
}