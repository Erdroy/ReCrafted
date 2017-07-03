using EnvDTE;
using Mono.Debugging.Soft;
using Mono.Debugging.VisualStudio;

namespace ReCrafted.Debugger
{
    internal class ReCraftedStartInfo : StartInfo
    {
        public ReCraftedStartInfo(SoftDebuggerStartArgs args, DebuggingOptions options, Project startupProject) : base(args, options, startupProject)
        {
        }
    }
}