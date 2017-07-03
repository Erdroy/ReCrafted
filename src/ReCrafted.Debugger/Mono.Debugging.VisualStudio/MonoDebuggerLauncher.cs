using Mono.Debugging.Client;
using Mono.Debugging.Soft;
using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace Mono.Debugging.VisualStudio
{
	public class MonoDebuggerLauncher
	{
		private readonly static ITracer tracer;

		private IDebugLauncher debugLauncher;

		private IProgress<string> progress;

		static MonoDebuggerLauncher()
		{
			MonoDebuggerLauncher.tracer = Tracer.Get<MonoDebuggerLauncher>();
		}

		public MonoDebuggerLauncher(IProgress<string> progress, IDebugLauncher debugLauncher = null)
		{
			ITracerExtensions.Verbose(MonoDebuggerLauncher.tracer, "Entering constructor for: {0}", new object[] { this });
			this.progress = progress;
			object obj = debugLauncher;
			if (obj == null)
			{
				obj = new DebugLauncher();
			}
			this.debugLauncher = (IDebugLauncher)obj;
		}

		private void ConfigureSession(SoftDebuggerSession debuggerSession, IProgress<string> progress)
		{
			debuggerSession.TargetExited += new EventHandler<TargetEventArgs>((object s, TargetEventArgs e) => debuggerSession.Dispose());
			debuggerSession.OutputWriter = (bool is_stderr, string text) => {
				if (progress != null)
				{
					if (text.Trim(new char[] { '\n', '\uFEFF' }).Length > 0)
					{
						progress.Report(text.TrimEnd(new char[] { '\n' }));
					}
				}
			};
			SoftDebuggerSession softDebuggerSession = debuggerSession;
			softDebuggerSession.ExceptionHandler = (ExceptionHandler)Delegate.Combine(softDebuggerSession.ExceptionHandler, new ExceptionHandler((Exception e) => {
				progress.Report(e.Message);
				ITracerExtensions.Error(MonoDebuggerLauncher.tracer, e);
				return true;
			}));
			debuggerSession.LogWriter = debuggerSession.OutputWriter;
		}

		public void StartSession(StartInfo startInfo, SoftDebuggerSession session)
		{
			ITracerExtensions.Verbose(MonoDebuggerLauncher.tracer, "Entering Start() for: {0}", new object[] { this });
			this.ConfigureSession(session, this.progress);
			this.debugLauncher.StartDebugger(session, startInfo);
			session.Run(startInfo, startInfo.SessionOptions);
		}
	}
}