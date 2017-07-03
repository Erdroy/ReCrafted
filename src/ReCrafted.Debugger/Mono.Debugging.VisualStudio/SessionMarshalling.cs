using Mono.Debugging.Soft;
using System;
using System.Runtime.CompilerServices;

namespace Mono.Debugging.VisualStudio
{
	public class SessionMarshalling : MarshalByRefObject
	{
		public SoftDebuggerSession Session
		{
			get;
			private set;
		}

		public Mono.Debugging.VisualStudio.StartInfo StartInfo
		{
			get;
			private set;
		}

		public SessionMarshalling(SoftDebuggerSession session, Mono.Debugging.VisualStudio.StartInfo startInfo)
		{
			this.Session = session;
			this.StartInfo = this.StartInfo;
		}
	}
}