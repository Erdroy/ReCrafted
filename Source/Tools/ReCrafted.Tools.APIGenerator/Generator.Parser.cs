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
                    // TODO: Struct parsing
                    break;
                case "API_FUNCTION":
                    ParseFunctionTag();
                    break;
                case "API_PROPERTY":
                    // TODO: Property parsing
                    break;
            }
        }

        private void ParseFunctionTag()
        {
            // API_FUNCTION(public, virtual, override, customName="Destroy")
            // void Destroy(int testParam) const override

            // TODO: Comments (read the token before current one)

            var desc = new FunctionDescription();

            // Parse tag arguments
            var tagArguments = ParseTagArguments();

            var token = _tokenizer.NextToken();
            var isStatic = token.Value == "static";

            if (isStatic)
                token = _tokenizer.NextToken();

            var isVirtual = token.Value == "virtual";

            // Set virtual modifier
            if (isVirtual)
                token = _tokenizer.NextToken();

            desc.ReturnType = token.Value;
            var functionName = _tokenizer.ExpectToken(TokenType.Identifier);

            var functionParams = ParseTagArguments(true);

            // TODO: Proper function parameter handling
            // const Type&, Type&, Type* etc. support is required.
            // Example:
            // 'const Vector3*', 'Vector3*' should become 'Vector3', and C++ should generate 'Vector3*' - this is because we cannot check the type
            // 'Vector3&' and 'const Vector3&' should become 'ref Vector3', and C++ should generate 'Vector3*'

            // TODO: Common type conversion: strings, integers etc.
            // Because: 'const char*', 'const String&' becomes string,
            // uint32_t becomes uint etc.

            for (var i = 0; i < functionParams.Count; i += 2)
            {
                var type = functionParams[i].Value;
                var name = functionParams[i + 1].Value;
                desc.Parameters.Add($"{type} {name}");
            }

            token = _tokenizer.NextToken();
            var isConst = token.Value == "const";

            if (isConst)
                token = _tokenizer.NextToken();

            var isOverride = token.Value == "override";

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                    case "internal":
                    case "private":
                    case "static":
                    case "virtual":
                    case "override":
                    case "abstract":
                        desc.Modifiers.Add(token.Value);
                        break;
                    case "byref":
                        break;
                    case "customName":
                        if (tagArguments.Count == i + 1 || tagArguments[i + 1].Type != TokenType.String)
                            throw new Exception("Class custom name doesn't follow with string!");

                        // Override name with new one
                        // TODO: Check the name for invalid characters etc.
                        desc.Name = tagArguments[i + 1].Value.Replace("\"", "");
                        break;
                    default:
                        Console.WriteLine(
                            $"WARNING: Unsupported modifier '{token.Value}' is passed to the API_FUNCTION tag of function {functionName.Value}!");
                        break;
                }
            }

            if (isOverride && !desc.Modifiers.Contains("override"))
                desc.Modifiers.Add("override");

            _functions.Add(desc);
        }

        private void ParseClassTag()
        {
            // API_CLASS(public, sealed..., customName="ClassTest", customNamespace="Namespace.Test")
            // class TestClass : public Object<TestClass>

            var desc = new ClassDescription();

            // TODO: Comments (read the token before current one)

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

            var isFinal = token.Value == "final";

            if (isFinal)
                token = _tokenizer.NextToken();

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

            if(isFinal && !desc.Modifiers.Contains("sealed"))
                desc.Modifiers.Add("sealed");

            _classDesc = desc;
        }
    }
}
