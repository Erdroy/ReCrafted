using Mono.Debugging.Client;
using Mono.Debugging.Soft;
using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace Mono.Debugging.VisualStudio
{
	public class DebuggerSession
	{
		private readonly static ITracer tracer;

		private SoftDebuggerSession debuggerSession;

		private IDebugLauncher debugLauncher;

		private StartInfo startInfo;

		private IProgress<string> progress;

		static DebuggerSession()
		{
			Mono.Debugging.VisualStudio.DebuggerSession.tracer = Tracer.Get<Mono.Debugging.VisualStudio.DebuggerSession>();
		}

		public DebuggerSession(StartInfo startInfo, IProgress<string> progress, SoftDebuggerSession session, IDebugLauncher debugLauncher = null)
		{
			ITracerExtensions.Verbose(Mono.Debugging.VisualStudio.DebuggerSession.tracer, "Entering constructor for: {0}", new object[] { this });
			this.progress = progress;
			this.startInfo = startInfo;
			this.debuggerSession = session;
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
				ITracerExtensions.Error(Mono.Debugging.VisualStudio.DebuggerSession.tracer, e);
				return true;
			}));
			debuggerSession.LogWriter = debuggerSession.OutputWriter;
		}

		public virtual void Exit()
		{
			this.debuggerSession.Exit();
		}

		public void Start()
		{
			ITracerExtensions.Verbose(Mono.Debugging.VisualStudio.DebuggerSession.tracer, "Entering Start() for: {0}", new object[] { this });
			this.ConfigureSession(this.debuggerSession, this.progress);
			this.debugLauncher.StartDebugger(this.debuggerSession, this.startInfo);
			this.debuggerSession.Run(this.startInfo, this.startInfo.SessionOptions);
		}
	}
}