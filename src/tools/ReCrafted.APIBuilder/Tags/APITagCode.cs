// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagCode : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1)
            {
                Console.WriteLine($"Invalid API_CODE token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (!APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_CODE token parameter (code) definition at line {APIBuilder.LineNumber}");
                return;
            }

            APITagMethod.Current.CodeLines.Add(APIBuilderUtils.GetStringValue(parameters[0]));
        }
    }
}
