// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.ProjectManager.Commands
{
    using APIBuilder = APIBuilder.APIBuilder;

    public class GenerateAPI : Command
    {
        public override void Execute()
        {
            APIBuilder.Build();
        }
    }
}
