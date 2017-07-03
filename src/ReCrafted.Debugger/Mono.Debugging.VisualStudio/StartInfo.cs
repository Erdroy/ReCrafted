using Clide.Solution;
using EnvDTE;
using Mono.CompilerServices.SymbolWriter;
using Mono.Debugging.Client;
using Mono.Debugging.Soft;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.CompilerServices;
using VSLangProj;

namespace Mono.Debugging.VisualStudio
{
	public class StartInfo : SoftDebuggerStartInfo, IStartInfo
	{
		public DebuggingOptions Options
		{
			get
			{
				return JustDecompileGenerated_get_Options();
			}
			set
			{
				JustDecompileGenerated_set_Options(value);
			}
		}

		private DebuggingOptions JustDecompileGenerated_Options_k__BackingField;

		public DebuggingOptions JustDecompileGenerated_get_Options()
		{
			return this.JustDecompileGenerated_Options_k__BackingField;
		}

		protected void JustDecompileGenerated_set_Options(DebuggingOptions value)
		{
			this.JustDecompileGenerated_Options_k__BackingField = value;
		}

		public DebuggerSessionOptions SessionOptions
		{
			get
			{
				return JustDecompileGenerated_get_SessionOptions();
			}
			set
			{
				JustDecompileGenerated_set_SessionOptions(value);
			}
		}

		private DebuggerSessionOptions JustDecompileGenerated_SessionOptions_k__BackingField;

		public DebuggerSessionOptions JustDecompileGenerated_get_SessionOptions()
		{
			return this.JustDecompileGenerated_SessionOptions_k__BackingField;
		}

		protected void JustDecompileGenerated_set_SessionOptions(DebuggerSessionOptions value)
		{
			this.JustDecompileGenerated_SessionOptions_k__BackingField = value;
		}

		public Project StartupProject
		{
			get
			{
				return JustDecompileGenerated_get_StartupProject();
			}
			set
			{
				JustDecompileGenerated_set_StartupProject(value);
			}
		}

		private Project JustDecompileGenerated_StartupProject_k__BackingField;

		public Project JustDecompileGenerated_get_StartupProject()
		{
			return this.JustDecompileGenerated_StartupProject_k__BackingField;
		}

		private void JustDecompileGenerated_set_StartupProject(Project value)
		{
			this.JustDecompileGenerated_StartupProject_k__BackingField = value;
		}

		public StartInfo(SoftDebuggerStartArgs start_args, DebuggingOptions options, Project startupProject) : base(start_args)
		{
			this.StartupProject = startupProject;
			this.Options = options;
			this.SessionOptions = this.CreateDebuggerSessionOptions();
			this.GetUserAssemblyNamesAndMaps();
		}

		private bool AddAssemblyToPathMap(string fileName, bool userAssembly)
		{
			if (base.AssemblyPathMap.ContainsValue(fileName) || !File.Exists(string.Concat(fileName, ".mdb")))
			{
				return false;
			}
			AssemblyName assemblyName = new AssemblyName(CecilHelper.GetAssemblyName(fileName));
			if (userAssembly)
			{
				base.UserAssemblyNames.Add(assemblyName);
			}
			base.AssemblyPathMap.Add(assemblyName.FullName, fileName);
			return true;
		}

		private bool AddProjectToPathMap(IProjectNode projectNode)
		{
			dynamic properties = projectNode.PropertiesFor(projectNode.Configuration.ActiveConfigurationName);
			dynamic fileName = Path.Combine(properties.targetDir, properties.TargetFileName);
			if (!File.Exists(fileName))
			{
				return true;
			}
			return (bool)this.AddAssemblyToPathMap(fileName, true);
		}

		private void AddReferencesToPathMap(Project project)
		{
			foreach (Reference reference in ProjectInterfaceConverters.GetReferences(project))
			{
				try
				{
					if (reference.SourceProject == null)
					{
						this.AddAssemblyToPathMap(reference.Path, this.IsMyCode(reference.Path));
					}
					else if (this.AddProjectToPathMap(AdapterFacade.AsProjectNode(AdapterFacade.Adapt(reference.SourceProject))))
					{
						this.AddReferencesToPathMap(reference.SourceProject);
					}
				}
				catch
				{
				}
			}
		}

		protected DebuggerSessionOptions CreateDebuggerSessionOptions()
		{
			EvaluationOptions evalOpts = EvaluationOptions.DefaultOptions;
			evalOpts.GroupPrivateMembers = true;
			evalOpts.GroupStaticMembers = true;
			evalOpts.FlattenHierarchy = false;
			evalOpts.AllowToStringCalls = false;
			return new DebuggerSessionOptions()
			{
				EvaluationOptions = evalOpts,
				ProjectAssembliesOnly = true
			};
		}

		protected DebuggingOptions CreateDefaultDebuggingOptions(bool longWait = true)
		{
			if (longWait)
			{
				return new DebuggingOptions()
				{
					EvaluationTimeout = new int?(60000),
					MemberEvaluationTimeout = new int?(180000),
					ModificationTimeout = new int?(120000),
					SocketTimeout = 0
				};
			}
			return new DebuggingOptions()
			{
				EvaluationTimeout = new int?(10000),
				MemberEvaluationTimeout = new int?(15000),
				ModificationTimeout = new int?(10000),
				SocketTimeout = 0
			};
		}

		protected void GetUserAssemblyNamesAndMaps()
		{
			base.UserAssemblyNames = new List<AssemblyName>();
			base.AssemblyPathMap = new Dictionary<string, string>();
			IProjectNode projectNode = AdapterFacade.AsProjectNode(AdapterFacade.Adapt(this.StartupProject));
			this.AddProjectToPathMap(projectNode);
			this.AddReferencesToPathMap(this.StartupProject);
		}

		private bool IsMyCode(string path)
		{
			if (!File.Exists(Path.ChangeExtension(path, "pdb")) || !File.Exists(string.Concat(path, ".mdb")))
			{
				return false;
			}
			MonoSymbolFile symbols = MonoSymbolFile.ReadSymbolFile(string.Concat(path, ".mdb"));
			return ((IEnumerable<SourceFileEntry>)symbols.Sources).Any<SourceFileEntry>((SourceFileEntry s) => File.Exists(s.FileName));
		}
	}
}