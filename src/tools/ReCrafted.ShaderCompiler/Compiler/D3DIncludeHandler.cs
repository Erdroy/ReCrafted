// ReCrafted (c) 2016-2019 Always Too Late

using System;
using System.IO;
using SharpDX.D3DCompiler;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public class D3DIncludeHandler : Include
    {
        private readonly string _sourceFileName;

        public D3DIncludeHandler(string sourceFileName)
        {
            _sourceFileName = sourceFileName;
        }

        public Stream Open(IncludeType type, string fileName, Stream parentStream)
        {
            if (type != IncludeType.Local)
                throw new Exception("Include type of " + type + " is not supported!");

            // TODO: Custom include directory, implement System include type

            var basePath = Path.GetDirectoryName(_sourceFileName);

            if (string.IsNullOrEmpty(basePath))
                throw new Exception("Invalid base path '" + basePath + "'!");

            var filePath = Path.Combine(basePath, fileName);

            if (Options.Current.Verbose)
                Console.WriteLine("Opening include " + type + " file '" + fileName + "' ('" + filePath + "').");

            return new FileStream(filePath, FileMode.Open);
        }

        public void Close(Stream stream)
        {
            stream.Close();
            stream.Dispose();
        }

        public void Dispose()
        {
        }

        public IDisposable Shadow { get; set; }
    }
}
