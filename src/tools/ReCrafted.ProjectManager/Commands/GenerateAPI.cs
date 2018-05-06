// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.ProjectManager.Commands
{
    using APIBuilder = APIBuilder.APIBuilder;

    public class GenerateAPI : Command
    {
        public override void Execute()
        {
            Console.WriteLine("Generating API...");
            APIBuilder.Build();
        }
    }
}
