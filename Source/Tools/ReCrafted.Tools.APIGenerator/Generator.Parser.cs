// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.Tools.APIGenerator.Description;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private void HandleApiTag(Token token)
        {
            switch (token.Value)
            {
                case "API_CLASS_BODY":
                    _tokenizer.ExpectAllTokens(new[] {TokenType.LeftParent, TokenType.RightParent});
                    break;
                case "API_CLASS":
                    ParseClassTag();
                    break;
                case "API_STRUCT":
                    break;
                case "API_FUNCTION":
                    break;
            }
        }

        private void ParseClassTag()
        {
            // API_CLASS(public, sealed..., customName="ClassTest", customNamespace="Namespace.Test")
            // class TestClass : public Object<TestClass>

            var desc = new ClassDescription();

            // Parse tag arguments
            var tagArguments = ParseTagArguments();

            // Skip until `class` keyword.
            _tokenizer.SkipUntil(TokenType.Identifier);

            // Parse class name
            var className = _tokenizer.ExpectToken(TokenType.Identifier);

            // Set class name
            desc.Name = className.Value;

            // Parse namespace
            desc.Namespace = ParseNamespace(_fileInput);

            if (Options.Current.Verbose)
                Console.WriteLine($"C++ class name: '{className.Value}'");

            // Parse class inheritance, support for single Type/Generic Type.
            var token = _tokenizer.NextToken();

            if (token.Type == TokenType.Colon)
            {
                // Current class does have inheritance defined
                var accessToken = _tokenizer.ExpectToken(TokenType.Identifier);
                var access = accessToken.Value;

                Token nameToken;
                if (access != "public")
                {
                    Console.WriteLine($"WARNING: C++ class '{className.Value}' inherits a type with non-public access");

                    // Parsed access token is name token, when the 'public' modifier is missing.
                    // (Look at the first comment in this function)
                    nameToken = accessToken;
                }
                else
                {
                    nameToken = _tokenizer.ExpectToken(TokenType.Identifier);
                }

                token = _tokenizer.NextToken();

                if (token.Type == TokenType.LeftAngleBracket)
                {
                    // This is a generic type
                    var genericType = _tokenizer.ExpectToken(TokenType.Identifier);
                    _tokenizer.ExpectToken(TokenType.RightAngleBracket);

                    var inheritType = $"{nameToken.Value}<{genericType.Value}>";

                    if (Options.Current.Verbose)
                        Console.WriteLine($"C++ class inherits '{inheritType}'");

                    desc.Inherits = inheritType;
                }
                else
                {
                    var inheritType = nameToken.Value;

                    if (Options.Current.Verbose)
                        Console.WriteLine($"C++ class inherits '{inheritType}'");

                    desc.Inherits = inheritType;
                }
            }
            else
            {
                // Must inherit!
                throw new Exception($"Class {className.Value} must inherit Object or any other type that inherits Object!");
            }

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                    case "internal":
                    case "private":
                    case "static":
                    case "sealed":
                    case "abstract":
                        desc.Modifiers.Add(token.Value);
                        break;
                    case "customName":
                        if (tagArguments.Count == i + 1 || tagArguments[i + 1].Type != TokenType.String)
                            throw new Exception("Class custom name doesn't follow with string!");

                        // Override name with new one
                        // TODO: Check the name for invalid characters etc.
                        desc.Name = tagArguments[i + 1].Value.Replace("\"", "");
                        break;
                    case "customNamespace":
                        if (tagArguments.Count == i + 1 || tagArguments[i + 1].Type != TokenType.String)
                            throw new Exception("Class custom namespace doesn't follow with string!");

                        // Override namespace with new one
                        // TODO: Check the namespace for invalid characters etc.
                        desc.Namespace = tagArguments[i + 1].Value.Replace("\"", "");
                        break;
                }
            }

            _classDesc = desc;
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
    }
}
