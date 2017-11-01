// ReCrafted © 2016-2017 Always Too Late

using System;
using System.Linq;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagParam : APITag
    {
        public string Name { get; set; }

        public string Type { get; set; }

        public bool IsRef { get; set; }

        public bool IsOut { get; set; }

        public APITagParam()
        {
            APITagMethod.Current.Parameters.Add(this);
        }

        public override void Process(string token, string[] parameters)
        {
            IsRef = parameters.Contains("REF");
            IsOut = parameters.Contains("OUT");

            if (parameters.Length != 2 && parameters.Length != 3)
            {
                Console.WriteLine($"Invalid API_PARAM* token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (!APIBuilderUtils.IsString(parameters[0]) || !APIBuilderUtils.IsString(parameters[1]))
            {
                Console.WriteLine($"Invalid API_PARAM* token parameter (text) definition at line {APIBuilder.LineNumber}");
                return;
            }

            Type = APIBuilderUtils.GetStringValue(parameters[0]);
            Name = APIBuilderUtils.GetStringValue(parameters[1]);
        }
    }
}
