using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using Mono.Debugging.Soft;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.Remoting;
using System.Runtime.Serialization.Formatters.Binary;

namespace Mono.Debugging.VisualStudio
{
	internal class DebugLauncher : IDebugLauncher
	{
		private static readonly ITracer Tracer;

		static DebugLauncher()
		{
			Tracer = System.Diagnostics.Tracer.Get<DebugLauncher>();
		}
        
		public bool StartDebugger(SoftDebuggerSession session, StartInfo startInfo)
		{
		    Tracer.Verbose("Entering Launch for: {0}", new object[] { this });
			var debugger = ServiceProvider.GlobalProvider.GetService<SVsShellDebugger, IVsDebugger4>();
			var sessionMarshaling = new SessionMarshalling(session, startInfo);
			var info = new VsDebugTargetInfo4()
			{
				dlo = (uint)DEBUG_LAUNCH_OPERATION.DLO_CreateProcess,
				bstrExe = "C:\\Program Files (x86)\\Mono\\bin\\mono.exe",
				bstrCurDir = "C:\\Program Files (x86)\\Mono\\bin\\",
				bstrArg = null,
				bstrRemoteMachine = null,
				fSendToOutputWindow = 0,
				guidPortSupplier = Guids.PortSupplierGuid,
				guidLaunchDebugEngine = Guids.EngineGuid,
				bstrPortName = null
            };
			using (var ms = new MemoryStream())
			{
				new BinaryFormatter().Serialize(ms, RemotingServices.Marshal(sessionMarshaling));
				info.bstrOptions = Convert.ToBase64String(ms.ToArray());
			}
			try
			{
				var results = new VsDebugTargetProcessInfo[1];
				debugger.LaunchDebugTargets4(1, new [] { info }, results);
			}
			catch (Exception exception)
			{
				Tracer.Error("Controller.Launch ()", exception);
				throw;
			}
			return true;
		}
	}
}