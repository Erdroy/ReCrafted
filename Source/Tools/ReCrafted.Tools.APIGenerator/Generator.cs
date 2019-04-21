// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Diagnostics;
using System.IO;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private readonly string _fileInput;
        private readonly string _fileCsOutput;
        private readonly string _fileCppOutput;

        private Tokenizer _tokenizer;

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

            if (source.Contains("API_"))
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
                            // TODO: Process!
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

        }

        public static bool GenerateClass(string inputFile, string csOutputFile, string cppOutputFile)
        {
            var perfCounter = new Stopwatch();
            perfCounter.Start();

            Console.WriteLine($"Generating API for '{inputFile}' C++ header file.");

            var generator = new Generator(inputFile, csOutputFile, cppOutputFile);

            try
            {
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
            }
            catch (Exception ex)
            {
                var errorMessage = Options.Current.Verbose ? ex.ToString() : ex.Message;
                Console.WriteLine("Failed to generate API file.\nError: \n" + errorMessage);
                return false;
            }

            // say bye and give the time that we spent here
            perfCounter.Stop();
            Console.WriteLine($"Done in {perfCounter.ElapsedMilliseconds} ms.");
            Console.WriteLine($"Successfully generated API for file '{inputFile}'. C# API File '{csOutputFile}', C++ API File '{cppOutputFile}'");
            return true;
        }
    }
}
