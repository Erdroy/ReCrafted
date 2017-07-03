using Mono.Debugging.Soft;
using System;

namespace Mono.Debugging.VisualStudio
{
	public interface IDebugLauncher
	{
		bool StartDebugger(SoftDebuggerSession session, StartInfo startInfo);
	}
}