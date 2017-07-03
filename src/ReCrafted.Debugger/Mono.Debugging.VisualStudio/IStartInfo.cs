using EnvDTE;
using Mono.Debugging.Client;

namespace Mono.Debugging.VisualStudio
{
	public interface IStartInfo
	{
		DebuggingOptions Options
		{
			get;
		}

		DebuggerSessionOptions SessionOptions
		{
			get;
		}

		Project StartupProject
		{
			get;
		}
	}
}