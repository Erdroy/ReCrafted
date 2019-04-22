// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using ReCrafted.Tools.APIGenerator.Description;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private readonly string _fileInput;
        private readonly string _fileCsOutput;
        private readonly string _fileCppOutput;

        private Tokenizer _tokenizer;
        private ClassDescription _class;
        //private StructDescription _structDesc;
        private readonly List<FunctionDescription> _functions = new List<FunctionDescription>();
        //private List<PropertyDescription> _properties;

        public Generator(string input, string csOutput, string cppOutput)
        {
            _fileInput = input;
            _fileCsOutput = csOutput;
            _fileCppOutput = cppOutput;

            if (!File.Exists(input))
                throw new Exception($"Input file {input} doesn't exists!");

            if(!input.EndsWith(".h"))
                throw new Exception($"Input file {input} is not an header file!");
        }

        public void Parse()
        {
            if (_tokenizer?.CurrentToken?.Type == TokenType.EndOfFile)
                throw new Exception("This file is already parsed!");

            if (Options.Current.Verbose)
                Console.WriteLine("Parsing " + Options.Current.InputFile);

            var source = File.ReadAllText(_fileInput);

            if (!source.Contains("API_"))
                throw new Exception($"Input file '{_fileInput}' doesn't contain any API tags!");

            _tokenizer = new Tokenizer();
            _tokenizer.Tokenize(source);

            do
            {
                // get and check current token
                var token = _tokenizer.NextToken();

                if (token == null)
                    continue;

                switch (token.Type)
                {
                    case TokenType.Identifier:
                        if (token.Value.StartsWith("API_"))
                        {
                            HandleApiTag(token);
                        }
                        break;

                    case TokenType.EndOfFile:
                        return;
                }
            }
            while (true);
        }

        public void Generate()
        {
            // TODO: generate code using T4
        }

        private string ParseNamespace(string fileName)
        {
            // D:\ReCrafted\Source\Engine\ReCrafted.Engine\APITestFile.h

            // Cut: 'D:\ReCrafted\Source\Engine\ReCrafted.Engine\'
            var nameSpaceStart = fileName.IndexOf("ReCrafted.Engine") + "ReCrafted.Engine".Length;
            var nameSpace = fileName.Substring(nameSpaceStart, fileName.Length - nameSpaceStart);

            // Now cut-out the file part
            var fileNameStart = nameSpace.LastIndexOf("\\");
            nameSpace = nameSpace.Substring(0, fileNameStart);

            // Split path and join with dots to create the namespace string
            nameSpace = string.Join(".", nameSpace.Split('\\'));

            // Add ReCrafted.API prefix to the namespace
            nameSpace = "ReCrafted.API" + nameSpace;

            return nameSpace;
        }

        private NativeTypeDescription ParseNativeType()
        {
            var type = new NativeTypeDescription();
            var token = _tokenizer.ExpectToken(TokenType.Identifier);

            // Possible types:
            // Vector3
            // const Vector3
            // const Vector3&
            // Vector3*
            // const Vector3*
            // Note: pointer-to-reference is not supported (Vector3&*).

            // Check 
            if (token.Value == "const")
            {
                type.IsConst = true;
                token = _tokenizer.ExpectToken(TokenType.Identifier);
            }

            // Current token is the base type
            type.BaseType = token.Value;

            token = _tokenizer.NextToken();

            if (token.Type == TokenType.Multiply)
            {
                type.ByPtr = true;
            }
            else if (token.Type == TokenType.And)
            {
                type.ByRef = true;
            }
            else
            {
                // Undo
                token = _tokenizer.PreviousToken();
            }

            return type;
        }

        private List<Token> ParseTagParameters(bool spaceSeparated = false)
        {
            var parameters = new List<Token>();

            _tokenizer.ExpectToken(TokenType.LeftParent);

            // expect series of parameters...
            var done = false;
            do
            {
                var token = _tokenizer.NextToken();

                // Exit when there is no any parameters
                if (token.Type == TokenType.RightParent)
                    return parameters;

                if (token.Type == TokenType.Identifier || token.Type == TokenType.String || token.Type == TokenType.Number)
                {
                    parameters.Add(token);

                    // check next token
                    var nextToken = _tokenizer.NextToken(spaceSeparated);
                    switch (nextToken.Type)
                    {
                        case TokenType.Whitespace:
                            break;
                        case TokenType.Equal:
                            break;
                        case TokenType.Comma:
                            break;
                        case TokenType.RightParent:
                            done = true;
                            break;

                        default:
                            throw new Exception("Expected right parent or next argument line " + _tokenizer.CurrentLine + ", but got " + token.Type + ".");
                    }
                }
            }
            while (!done);

            // return arguments
            return parameters;
        }

        private IEnumerable<FunctionDescription.Param> ParseFunctionParameters()
        {
            var parameters = new List<FunctionDescription.Param>();

            _tokenizer.ExpectToken(TokenType.LeftParent);

            // (int test, const Vector3& vector, Vector3* vPtr)

            // expect series of arguments...
            var done = false;
            var currentParam = new FunctionDescription.Param();
            do
            {
                var token = _tokenizer.NextToken();

                // Exit when there is no any parameters
                if (token.Type == TokenType.RightParent)
                    return parameters;

                // Undo
                _tokenizer.PreviousToken();

                currentParam.NativeType = ParseNativeType();
                currentParam.Name = _tokenizer.ExpectToken(TokenType.Identifier).Value;

                // Expect
                token = _tokenizer.ExpectAnyTokens(new[] { TokenType.Comma, TokenType.RightParent });

                if (token.Type == TokenType.Comma)
                {
                    parameters.Add(currentParam);
                    currentParam = new FunctionDescription.Param();
                }
                else if (token.Type == TokenType.RightParent)
                {
                    parameters.Add(currentParam);
                    done = true;
                }
            }
            while (!done);

            // return arguments
            return parameters;
        }

        private void TranslateTypes() // ???
        {
            // TODO: Proper function parameter handling
            // const Type&, Type&, Type* etc. support is required.
            // Example:
            // 'const Vector3*', 'Vector3*' should become 'Vector3', and C++ should generate 'Vector3*' - this is because we cannot check the type
            // 'Vector3&' and 'const Vector3&' should become 'ref Vector3', and C++ should generate 'Vector3*'

            // TODO: Common type conversion: strings, integers etc.
            // Because: 'const char*', 'const String&' becomes string,
            // uint32_t becomes uint etc.
        }

        public static bool GenerateClass(string inputFile, string csOutputFile, string cppOutputFile)
        {
            var perfCounter = new Stopwatch();
            perfCounter.Start();

            Console.WriteLine($"Generating API for '{inputFile}' C++ header file.");

            var generator = new Generator(inputFile, csOutputFile, cppOutputFile);

#if !DEBUG
            try
            {
#endif
                var perfCounterParse = new Stopwatch();
                perfCounterParse.Start();
                generator.Parse();
                perfCounterParse.Stop();
                Console.WriteLine($"Parsing done in {perfCounterParse.ElapsedMilliseconds} ms.");

                perfCounterParse.Reset();
                perfCounterParse.Start();
                generator.Generate();
                perfCounterParse.Stop();
                Console.WriteLine($"Generation done in {perfCounterParse.ElapsedMilliseconds} ms.");
#if !DEBUG
            }
            catch (Exception ex)
            {
                var errorMessage = Options.Current.Verbose ? ex.ToString() : ex.Message;
                Console.WriteLine("Failed to generate API file.\nError: \n" + errorMessage);
                return false;
            }
#endif

            // say bye and give the time that we spent here
            perfCounter.Stop();
            Console.WriteLine($"Done in {perfCounter.ElapsedMilliseconds} ms.");
            Console.WriteLine($"Successfully generated API for file '{inputFile}'. C# API File '{csOutputFile}', C++ API File '{cppOutputFile}'");
            return true;
        }
    }
}
