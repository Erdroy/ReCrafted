// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagFile : APITag
    {
        public static APITagFile Current;

        public string SourceFileName { get; set; }

        public string TargetFileName { get; set; }

        public List<APITagUsing> Usings = new List<APITagUsing>();

        public List<APITagClass> Classes = new List<APITagClass>();

        //public List<APITagEnum> Enums = new List<APITagEnum>();
        //public List<APITagStruct> Structs = new List<APITagStruct>();
        //public List<APITagDelegate> Delegates = new List<APITagDelegate>();

        public APITagFile()
        {
            Current = this;
            APIBuilder.Files.Add(this);
        }

        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1 || !APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_FILE token definition at line {APIBuilder.LineNumber}");
                return;
            }

            var targetName = APIBuilderUtils.GetStringValue(parameters[0]);
            
            if (!targetName.Contains(".cs") || string.IsNullOrEmpty(targetName))
            {
                Console.WriteLine($"Invalid API_FILE target file at line {APIBuilder.LineNumber}");
                return;
            }

            TargetFileName = targetName;
        }
    }
}
