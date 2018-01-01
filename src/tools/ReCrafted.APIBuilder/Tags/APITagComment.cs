// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagComment : APITag
    {
        public static APITagComment Current;

        public string Text { get; set; }

        public APITagComment()
        {
            Current = this;
        }

        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1)
            {
                Console.WriteLine($"Invalid API_COMMENT token definition at line {APIBuilder.LineNumber}");
                return;
            }
            
            if (!APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_COMMENT token parameter (text) definition at line {APIBuilder.LineNumber}");
                return;
            }

            Text = APIBuilderUtils.GetStringValue(parameters[0]);
        }
    }
}
