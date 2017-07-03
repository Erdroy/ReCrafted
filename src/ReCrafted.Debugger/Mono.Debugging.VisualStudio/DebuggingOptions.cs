using System;
using System.Runtime.CompilerServices;

namespace Mono.Debugging.VisualStudio
{
	public class DebuggingOptions
	{
		public int? EvaluationTimeout
		{
			get;
			set;
		}

		public int? MemberEvaluationTimeout
		{
			get;
			set;
		}

		public int? ModificationTimeout
		{
			get;
			set;
		}

		public int SocketTimeout
		{
			get;
			set;
		}

		public DebuggingOptions()
		{
		}
	}
}