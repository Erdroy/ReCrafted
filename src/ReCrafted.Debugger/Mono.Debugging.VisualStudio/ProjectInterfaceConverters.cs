using EnvDTE;
using Microsoft.VisualStudio;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using VSLangProj;

namespace Mono.Debugging.VisualStudio
{
	internal static class ProjectInterfaceConverters
	{
		private readonly static Guid VsSolutionFolder;

		static ProjectInterfaceConverters()
		{
			ProjectInterfaceConverters.VsSolutionFolder = new Guid("{66A26720-8FB5-11D2-AA7E-00C04F688DDE}");
		}

		public static void AddReference(this Project project, Project toProject)
		{
			VSProject proj = ProjectInterfaceConverters.ToVSProject(project);
			proj.References.AddProject(toProject);
		}

		public static IVsHierarchy[] AllProjects(this IVsSolution solution)
		{
			IEnumHierarchies ppEnum;
			List<IVsHierarchy> projects = new List<IVsHierarchy>();
			if (solution == null)
			{
				return projects.ToArray();
			}
			Guid tempGuid = Guid.Empty;
			solution.GetProjectEnum(27, ref tempGuid, out ppEnum);
			if (ppEnum != null)
			{
				uint actualResult = 0;
				IVsHierarchy[] nodes = new IVsHierarchy[1];
				while (ppEnum.Next(1, nodes, out actualResult) == 0)
				{
					projects.Add(nodes[0]);
				}
			}
			return projects.ToArray();
		}

		public static IVsHierarchy GetCurrentHierarchy(this System.IServiceProvider provider)
		{
			Project selectedProject = provider.GetSelectedProject();
			if (selectedProject == null)
			{
				return null;
			}
			return selectedProject.ToHierarchy();
		}

		public static Guid GetProjectGuid(this Project project)
		{
			Guid guid;
			IVsHierarchy hierarchy = project.ToHierarchy();
			if (hierarchy != null && ErrorHandler.Succeeded((Package.GetGlobalService(typeof(SVsSolution)) as IVsSolution2).GetGuidOfProject(hierarchy, out guid)))
			{
				return guid;
			}
			return Guid.Empty;
		}

		public static Project[] GetProjects(this IVsSolution solution, string projectKind = null)
		{
			IEnumHierarchies ppEnum;
			object obj;
			List<Project> projects = new List<Project>();
			if (solution == null)
			{
				return projects.ToArray();
			}
			Guid tempGuid = Guid.Empty;
			solution.GetProjectEnum(27, ref tempGuid, out ppEnum);
			if (ppEnum != null)
			{
				uint actualResult = 0;
				IVsHierarchy[] nodes = new IVsHierarchy[1];
				while (ppEnum.Next(1, nodes, out actualResult) == 0)
				{
					nodes[0].GetProperty((uint)2, -2027, out obj);
					Project project = obj as Project;
					if (project == null)
					{
						continue;
					}
					if (!string.IsNullOrEmpty(projectKind))
					{
						if (!projectKind.Equals(project.Kind, StringComparison.InvariantCultureIgnoreCase))
						{
							continue;
						}
						projects.Add(project);
					}
					else
					{
						projects.Add(project);
					}
				}
			}
			return projects.ToArray();
		}

		public static IEnumerable<Reference> GetReferences(Project project)
		{
			foreach (Reference reference in ProjectInterfaceConverters.ToVSProject(project).References)
			{
				yield return reference;
			}
		}

		public static Project GetSelectedProject(this System.IServiceProvider provider)
		{
			DTE vs = (DTE)provider.GetService(typeof(DTE));
			if (vs == null)
			{
				throw new InvalidOperationException("DTE not found.");
			}
			SelectedItem item = vs.SelectedItems.Item(1);
			if (item.Project != null)
			{
				return item.Project;
			}
			if (item.ProjectItem == null)
			{
				return null;
			}
			return item.ProjectItem.ContainingProject;
		}

		public static bool IsSolutionFolder(this Project item)
		{
			return (new Guid(item.Kind)).Equals(ProjectInterfaceConverters.VsSolutionFolder);
		}

		public static bool References(this Project project, Project reference)
		{
			return ProjectInterfaceConverters.GetReferences(project).Any<Reference>((Reference r) => r.SourceProject == reference);
		}

		public static Project ToDteProject(this IVsHierarchy hierarchy)
		{
			if (hierarchy == null)
			{
				throw new ArgumentNullException("hierarchy");
			}
			object prjObject = null;
			if (hierarchy.GetProperty(2, -2027, out prjObject) != 0)
			{
				return null;
			}
			return (Project)prjObject;
		}

		public static Project ToDteProject(this IVsProject project)
		{
			if (project == null)
			{
				throw new ArgumentNullException("project");
			}
			return (project as IVsHierarchy).ToDteProject();
		}

		public static Guid ToGuid(this Project project)
		{
			System.IServiceProvider serviceProvider = new ServiceProvider(project.DTE as Microsoft.VisualStudio.OLE.Interop.IServiceProvider);
			IVsSolution solution = serviceProvider.GetService(typeof(SVsSolution)) as IVsSolution;
			IVsHierarchy hierarchy = project.ToHierarchy();
			Guid projectGuid = new Guid();
			if (hierarchy != null)
			{
				solution.GetGuidOfProject(hierarchy, out projectGuid);
			}
			return projectGuid;
		}

		public static IVsHierarchy ToHierarchy(this Project project)
		{
			IVsHierarchy hierarchy;
			IVsHierarchy vsHierarchy;
			if (project == null)
			{
				throw new ArgumentNullException("project");
			}
			try
			{
				if ((Package.GetGlobalService(typeof(SVsSolution)) as IVsSolution2).GetProjectOfUniqueName(project.UniqueName, out hierarchy) != 0)
				{
					return null;
				}
				else
				{
					vsHierarchy = hierarchy;
				}
			}
			catch (NotImplementedException notImplementedException)
			{
				vsHierarchy = null;
			}
			return vsHierarchy;
		}

		public static IVsBuildPropertyStorage ToVsBuildPropertyStorage(this Project project)
		{
			return project.ToHierarchy() as IVsBuildPropertyStorage;
		}

		public static IVsProject3 ToVsProject(this Project project)
		{
			if (project == null)
			{
				throw new ArgumentNullException("project");
			}
			IVsProject3 vsProject = project.ToHierarchy() as IVsProject3;
			if (vsProject == null)
			{
				throw new ArgumentException("Project is not a VS project.");
			}
			return vsProject;
		}

		private static VSProject ToVSProject(Project project)
		{
			VSProject proj = project.Object as VSProject;
			if (proj == null)
			{
				throw new ArgumentException("project is not a VSLangProj.VSProject");
			}
			return proj;
		}
	}
}