// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Newtonsoft.Json;
using ReCrafted.ShaderCompiler.Tokenizer;
using ReCrafted.ShaderCompiler.Description;
using SharpDX.D3DCompiler;
using SharpDX.Direct3D;

namespace ReCrafted.ShaderCompiler.Compiler
{
    using ShaderDescription = Description.ShaderDescription;

    public partial class ShaderCompiler : IDisposable
    {
        private readonly string _inputFile;
        private readonly string _outputFile;

        private IEnumerator<Token> _tokenEnumerator;

        private Token _previousToken;
        private int _currentLineNumber;
        private bool _ignoreSource;

        private FunctionAttribute _lastAttribute;

        public string ShaderName;
        public string ShaderDescription;
        
        public StringBuilder ProcessedSourceCode = new StringBuilder();
        public List<string> Includes = new List<string>();
        public List<ShaderPass> Passes = new List<ShaderPass>();
        public List<ShaderUniformBuffer> Buffers = new List<ShaderUniformBuffer>();
        public List<ShaderUniform> Uniforms = new List<ShaderUniform>();

        public ShaderCompiler(string input, string output)
        {
            _inputFile = input;
            _outputFile = output;

            if (!File.Exists(_inputFile))
                throw new Exception("File " + _inputFile + " doesn't exists!");
        }

        public void Tokenize()
        {
            if (_tokenEnumerator != null)
                throw new Exception("This shader is already tokenized!");

            if (Options.Current.Verbose)
                Console.WriteLine("Parsing " + Options.Current.InputFile);

            using (var file = new FileStream(_inputFile, FileMode.Open))
            {
                using (var reader = new StreamReader(file))
                {
                    var sourceCode = reader.ReadToEnd();

                    if (Options.Current.Verbose)
                        Console.WriteLine("Tokenizing total " + sourceCode.Length + " characters");

                    var tokens = Tokenizer.Tokenizer.Run(sourceCode);
                    _tokenEnumerator = tokens.GetEnumerator();
                }
            }
        }

        public void Parse()
        {
            if (_tokenEnumerator.Current?.Type==TokenType.EndOfFile)
               throw new Exception("This shader is already parsed!");

            do
            {
                // get and check current token
                var token = NextToken();

                if (token == null)
                    continue;

                switch (token.Type)
                {
                    case TokenType.LeftBracket:
                    {
                        // only our custom attributes use these brackets
                        // start ignoring source code right there
                        IgnoreSource(true);
                        HandleAttribute();
                        IgnoreSource(false);
                        break;
                    }

                    case TokenType.Preprocessor:
                        HandlePreprocessor();
                        break;

                    case TokenType.Identifier:
                        // handle identifiers
                        HandleIdentifier(token);
                        break;
                    case TokenType.EndOfFile:
                        return;
                }
            }
            while (true);
        }

        public void Compile()
        {
            // Sometime...
            // TODO: Shader permutation
            // TODO: More shader types support
            // TODO: Per-function profile set (SetVertexShader(VSMain, 5.0))

            // We don't need semantics for D3D as it will get it using reflection. What about other planned backends?

            var sourceCode = ProcessedSourceCode.ToString();

            if (Options.Current.Verbose)
                Console.WriteLine("Pre-processing...");

            // pre process source code (this will add include source code etc.)
            var preprocessedSource = ShaderBytecode.Preprocess(sourceCode, new[]
            {
                new ShaderMacro("GFXL_SHADER", "1"),
                new ShaderMacro("GFXL_VERSION", "1")
            }, new D3DIncludeHandler(_inputFile), out var compilationErrors, _inputFile);

            if(!string.IsNullOrEmpty(compilationErrors))
                throw new Exception(compilationErrors);
            
            // compile for DirectX
            CompileD3D(ref preprocessedSource);

            // TODO: compile for Vulkan / OpenGL or whatever else...

        }

        public void Save()
        {
            if (Options.Current.Verbose)
                Console.WriteLine("Saving output to '" + _outputFile + "'.");

            var desc = new ShaderDescription
            {
                Name = ShaderName,
                Description = ShaderDescription,
                Passes = Passes.ToArray(),
                UniformBuffers = Buffers.ToArray()
            };

            // serialize shader desc into json string
            var jsonData = JsonConvert.SerializeObject(desc, Options.Current.CuteJson ? Formatting.Indented : Formatting.None);

            // save into the file
            using (var file = new FileStream(_outputFile, FileMode.Create))
            {
                using (var stream = new StreamWriter(file))
                {
                    stream.Write(jsonData);
                }
            }
        }

        public void Dispose()
        {
            _tokenEnumerator.Dispose();
        }

        private void IgnoreSource(bool ignore)
        {
            _ignoreSource = ignore;

            if (!ignore)
                _previousToken = null;
        }

        private void CompileD3D(ref string sourceCode)
        {
            if (Options.Current.Verbose)
                Console.WriteLine("Compiling for DirectX...");

            foreach (var pass in Passes)
            {
                if (Options.Current.Verbose)
                    Console.WriteLine("Compiling pass '" + pass.Name + "'...");
                
                pass.VSByteCode = CompileShader(ref sourceCode, pass.VSFunction, pass.Profile, "vs");
                pass.PSByteCode = CompileShader(ref sourceCode, pass.PSFunction, pass.Profile, "ps");
                pass.CSByteCode = CompileShader(ref sourceCode, pass.CSFunction, pass.Profile, "cs");
            }
        }

        private byte[] CompileShader(ref string sourceCode, string entryPoint, int profile, string shaderPrefix)
        {
            if (string.IsNullOrEmpty(entryPoint))
                return null;

            var result = ShaderBytecode.Compile(sourceCode, entryPoint, shaderPrefix + "_" + profile + "_0");

            if(result.HasErrors || !string.IsNullOrEmpty(result.Message))
                throw new Exception(result.Message + " Error Code: " + result.ResultCode);

            var byteCode = result.Bytecode;

            if (Options.Current.DisassembleOutput)
            {
                var output = _outputFile + "_" + shaderPrefix + ".disasm";

                if (Options.Current.Verbose)
                    Console.WriteLine("Disassembling shader function to " + output);

                var data = byteCode.Disassemble(DisassemblyFlags.EnableInstructionOffset | DisassemblyFlags.EnableInstructionNumbering);
                File.WriteAllText(output, data);
            }

            return byteCode.Data;
        }
    }
}
