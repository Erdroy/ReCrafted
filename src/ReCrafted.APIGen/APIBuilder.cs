// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.IO;
using ReCrafted.APIGen.Tags;

namespace ReCrafted.APIGen
{
    internal class APIBuilder
    {
        private Dictionary<string, APITag> _apiTags = new Dictionary<string, APITag>
        {
            { "API_FILE", new APITagFile() },
            { "API_FILE_END", new APITagFileEnd() },
            { "API_USING", new APITagUsing() },

            { "API_COMMENT", new APITagComment() },

            { "API_CLASS", new APITagClass() },
            { "API_CLASS_INHERIT", new APITagClass() },
            { "API_CLASS_END", new APITagClassEnd() },

            { "API_METHOD", new APITagMethod() },
            { "API_METHOD_END", new APITagMethodEnd() },
            { "API_PARAM", new APITagParam() },
            { "API_PARAM_REF", new APITagParam() },
            { "API_PARAM_OUT", new APITagParam() },
            { "API_RETURN", new APITagReturn() },
        };
        
        public string GenerateCode(string fileName, out string targetName)
        {
            var filedesc = Parse(fileName);

            targetName = filedesc.TargetFileName;

            var generator = new CodeGenerator
            {
                Session = new Dictionary<string, object>
                {
                    { "Desc", filedesc }
                }
            };
            generator.Initialize();
            
            return generator.TransformText();
        }

        private APITagFile Parse(string fileName)
        {
            var fileTag = new APITagFile
            {
                SourceFileName = Path.GetFileName(fileName)
            };

            var sourceCode = File.ReadAllText(fileName);
            var lines = sourceCode.Split('\n');

            var lineNumber = 0;
            foreach (var lineSource in lines)
            {
                lineNumber++;

                var line = lineSource.Trim();

                if (!line.StartsWith("API_") || line.StartsWith("API_BIND"))
                    continue;

                try
                {
                    var paramsStart = line.IndexOf("(", StringComparison.Ordinal);
                    var token = line.Substring(0, paramsStart);

                    if (!_apiTags.ContainsKey(token))
                    {
                        Console.WriteLine($"Unknown token '{token}' at line {lineNumber}");
                        continue;
                    }

                    var tag = _apiTags[token];

                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Line {lineNumber} caused an exception {ex}");
                }
            }

            return fileTag;
        }
    }
}
