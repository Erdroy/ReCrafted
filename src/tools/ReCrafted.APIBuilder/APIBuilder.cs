// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using ReCrafted.APIBuilder.Tags;

namespace ReCrafted.APIBuilder
{
    public class APIBuilder
    {
        private readonly Dictionary<string, Type> _apiTags = new Dictionary<string, Type>
        {
            { "API_FILE", typeof(APITagFile) },
            { "API_FILE_END", typeof(APITagFileEnd) },
            { "API_USING", typeof(APITagUsing) },

            { "API_COMMENT", typeof(APITagComment) },

            { "API_CLASS", typeof(APITagClass) },
            { "API_CLASS_END", typeof(APITagClassEnd) },

            { "API_PROPERTY", typeof(APITagProperty) },
            { "API_PROPERTY_END", typeof(APITagPropertyEnd) },

            { "API_METHOD", typeof(APITagMethod) },
            { "API_METHOD_END", typeof(APITagMethodEnd) },
            { "API_CODE", typeof(APITagCode) },
            { "API_PARAM", typeof(APITagParam) },
            { "API_RETURN", typeof(APITagReturn) },

            { "API_BIND", typeof(APITagNull) },
            { "API_DEFINE_OBJECT_CREATOR", typeof(APITagNull) },
            { "API_REGISTER_OBJECT", typeof(APITagNull) }
        };
        
        public static List<APITagFile> Files = new List<APITagFile>();
        public static string CurrentFileName;

        public void Build(string sourceFile, string currentDir)
        {
            foreach (var desc in Parse(sourceFile))
            {
                var targetFileName = desc.TargetFileName;
                desc.SourceFileName = Path.GetFileName(sourceFile);

                var targetFile = currentDir + "\\src\\ReCrafted.API\\" + targetFileName;

                Directory.CreateDirectory(Path.GetDirectoryName(targetFile));

                var targetFileInfo = new FileInfo(targetFile);
                var sourceFileInfo = new FileInfo(sourceFile);

                if (targetFileInfo.LastWriteTime < sourceFileInfo.LastWriteTime)
                {
                    var generator = new CodeGenerator
                    {
                        Session = new Dictionary<string, object>
                        {
                            { "Desc", desc }
                        }
                    };
                    generator.Initialize();

                    var code = generator.TransformText();

                    File.WriteAllText(targetFile, code);
                    targetFileInfo.LastWriteTime = sourceFileInfo.LastWriteTime;

                    generator.ClearIndent();
                }
            }

            Files.Clear();
        }

        private APITagFile[] Parse(string fileName)
        {
            CurrentFileName = fileName;

            var sourceCode = File.ReadAllText(fileName);
            var lines = sourceCode.Split('\n');

            LineNumber = 0;
            foreach (var lineSource in lines)
            {
                LineNumber++;

                var line = lineSource.Trim();
                LineText = line;
                
                if (!line.StartsWith("API_"))
                    continue;

                try
                {
                    var paramsStart = line.IndexOf("(", StringComparison.Ordinal);
                    var token = line.Substring(0, paramsStart);

                    if (!_apiTags.ContainsKey(token))
                    {
                        Console.WriteLine($"Unknown token '{token}' at line {LineNumber}");
                        continue;
                    }

                    var tag = _apiTags[token];

                    if (tag == typeof(APITagNull))
                        continue;

                    var parameters = APIBuilderUtils.GetParameters(line);

                    var inst = (APITag)Activator.CreateInstance(tag);
                    inst.Process(token, parameters);
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"Line {LineNumber} caused an exception {ex}");
                }
            }

            return Files.ToArray();
        }

        public static void Build()
        {
            var timer = new Stopwatch();
            timer.Start();

            var currentDir = Environment.CurrentDirectory;
            if (currentDir.Contains("tools"))
                currentDir += "\\..";

            var apiSourceFiles = Directory.GetFiles(currentDir + "\\src\\ReCrafted.Engine\\", "*.API.cpp", SearchOption.AllDirectories);

            var builder = new APIBuilder();
            foreach (var sourceFile in apiSourceFiles)
            {
                Console.WriteLine("Processing: " + Path.GetFileName(sourceFile));
                builder.Build(sourceFile, currentDir);
            }

            timer.Stop();
            Console.WriteLine($"Generated in {timer.ElapsedMilliseconds} ms.");
        }

        public static int LineNumber { get; private set; }

        public static string LineText { get; private set; }
    }
}
