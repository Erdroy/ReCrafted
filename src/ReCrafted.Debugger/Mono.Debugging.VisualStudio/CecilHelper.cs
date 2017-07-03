using Mono.Cecil;
using Mono.Collections.Generic;
using System;
using System.Collections.Generic;

namespace Mono.Debugging.VisualStudio
{
	internal static class CecilHelper
	{
		public static string GetAssemblyName(string filename)
		{
			return AssemblyDefinition.ReadAssembly(filename).Name.FullName;
		}

		public static List<string> GetDependencies(string filename, bool fullname)
		{
			List<string> deps = new List<string>();
			foreach (AssemblyNameReference r in AssemblyDefinition.ReadAssembly(filename).MainModule.AssemblyReferences)
			{
				deps.Add((fullname ? r.FullName : r.Name));
			}
			return deps;
		}
	}
}