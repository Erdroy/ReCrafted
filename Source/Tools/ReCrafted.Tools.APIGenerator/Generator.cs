// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using System.IO;
using ReCrafted.Tools.APIGenerator.Descriptions;
using ReCrafted.Tools.APIGenerator.Templates;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private Tokenizer _tokenizer;
        private ClassDescription _class;
        private readonly List<FunctionDescription> _functions = new List<FunctionDescription>();
        private readonly List<PropertyDescription> _properties = new List<PropertyDescription>();
        public readonly Dictionary<string, string> CustomTranslation = new Dictionary<string, string>();


        public Generator(string input)
        {
            InputFile = input;

            if (!File.Exists(input))
                throw new Exception($"Input file {input} doesn't exists!");

            if(!input.EndsWith(".h"))
                throw new Exception($"Input file {input} is not an header file!");

            Instance = this;
        }

        private TypeDescription ParseNativeType()
        {
            var type = new TypeDescription();
            var token = _tokenizer.ExpectToken(TokenType.Identifier);

            // Possible types:
            // Vector3
            // const Vector3
            // const Vector3&
            // Vector3*
            // const Vector3*
            // Note: pointer-to-reference is not supported (Vector3&*).
            // Type<void>
            // const Type<void, ...>&

            // Check 
            if (token.Value == "const")
            {
                type.IsConst = true;

                _tokenizer.SkipUntil(TokenType.Identifier);

                token = _tokenizer.CurrentToken;
            }

            // Current token is the base type
            type.BaseType = token.Value;

            token = _tokenizer.NextToken();

            // Handle Generic/Template type
            if (token.Type == TokenType.LeftAngleBracket)
            {
                // TODO: Start reading template parameters

                do
                {
                    type.GenericTypes.Add(ParseNativeType());
                    token = _tokenizer.NextToken();
                } while (token.Type != TokenType.RightAngleBracket);

                token = _tokenizer.NextToken();
            }

            if (token.Type == TokenType.Multiply)
            {
                type.ByPtr = true;
            }
            else if (token.Type == TokenType.And)
            {
                type.ByRef = true;
            }
            else if (token.Type == TokenType.Colon)
            {
                _tokenizer.ExpectToken(TokenType.Colon);
                type.BaseType += "::";
                type.BaseType += _tokenizer.ExpectToken(TokenType.Identifier).Value;
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

        private string[] ParseXmlCommentAbove()
        {
            var lines = new List<string>();

            while (_tokenizer.PreviousToken().Type == TokenType.CommentSingleLine)
            {
                var commentLine = _tokenizer.CurrentToken.Value;

                commentLine = commentLine.Trim();

                lines.Insert(0, commentLine);
            }

            // Rewind
            for (var i = 0; i < lines.Count + 1; i++)
                _tokenizer.NextToken();

            return lines.ToArray();
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

                currentParam.Type = ParseNativeType();
                currentParam.Name = _tokenizer.ExpectToken(TokenType.Identifier).Value;

                if (_tokenizer.NextToken().Type == TokenType.Equal)
                {
                    // Parse default value
                    var defaultValueToken = _tokenizer.NextToken();

                    // There are 3 supported options:
                    // number: 1.0f
                    // string: "str"
                    // identifier: true
                    
                    currentParam.DefaultValue = defaultValueToken.Value;
                }
                else
                    _tokenizer.PreviousToken();

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

        public void Parse()
        {
            if (_tokenizer?.CurrentToken?.Type == TokenType.EndOfFile)
                throw new Exception("This file is already parsed!");

            Console.WriteLine("Parsing " + InputFile);

            var source = File.ReadAllText(InputFile);

            if (!source.Contains("API_"))
                throw new Exception($"Input file '{InputFile}' doesn't contain any API tags!");

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

        public void Generate(string csOutput, string cppOutput)
        {
            // Generate C# class code
            var classGenerator = new CSharpTemplate
            {
                Session = new Dictionary<string, object>
                {
                    { "Class", _class },
                    { "Functions", _functions },
                    { "Properties", _properties }
                }
            };
            classGenerator.Initialize();

            File.WriteAllText(csOutput, classGenerator.TransformText());

            // Generate C++ proxy code
            var proxyGenerator = new CPlusPlusTemplate
            {
                Session = new Dictionary<string, object>
                {
                    { "Class", _class },
                    { "Functions", _functions },
                    { "Properties", _properties }
                }
            };
            proxyGenerator.Initialize();

            File.WriteAllText(cppOutput, proxyGenerator.TransformText());
        }

        public static string ParseNamespace(string fileName)
        {
            // D:\ReCrafted\Source\Engine\ReCrafted.Engine\APITestFile.h

            // Cut: 'D:\ReCrafted\Source\Engine\ReCrafted.Engine\'
            var nameSpaceStart = fileName.IndexOf("ReCrafted.Engine", StringComparison.InvariantCultureIgnoreCase) + "ReCrafted.Engine".Length;
            var nameSpace = fileName.Substring(nameSpaceStart, fileName.Length - nameSpaceStart);

            // Now cut-out the file part
            var fileNameStart = nameSpace.LastIndexOf("\\", StringComparison.InvariantCultureIgnoreCase);
            nameSpace = nameSpace.Substring(0, fileNameStart);

            // Split path and join with dots to create the namespace string
            nameSpace = string.Join(".", nameSpace.Split('\\'));

            // Add ReCrafted.API prefix to the namespace
            nameSpace = "ReCrafted.API" + nameSpace;

            return nameSpace;
        }

        public string InputFile { get; }

        public string InputFileName => Path.GetFileNameWithoutExtension(InputFile);

        public string Namespace => _class != null ? _class.Namespace : ParseNamespace(InputFile);

        public List<string> Usings = new List<string>();

        public static void GenerateRuntimeInitialization(ClassList classList, string outputFile)
        {
            var initRuntimeGen = new InitRuntime
            {
                Session = new Dictionary<string, object>
                {
                    { "ClassList", classList },
                }
            };
            initRuntimeGen.Initialize();

            File.WriteAllText(outputFile, initRuntimeGen.TransformText());
        }

        public static Generator Instance { get; private set; }
    }
}
