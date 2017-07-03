
using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.Shell;
using Mono.Debugging.Client;

namespace ReCrafted.Debugger
{
    [PackageRegistration(UseManagedResourcesOnly = true)]
    [InstalledProductRegistration("ReCrafted.Debugger", "ReCrafted.Debugger", "1.0", IconResourceID = 400)] // Info on this package for Help/About
    [Guid(PackageGuidString)]
    [ProvideProjectFactory(typeof (ReCraftedProjectFactory), "ReCrafted.Debugger", null, null, null, null, LanguageVsTemplate = "CSharp", TemplateGroupIDsVsTemplate = "ReCrafted.Debugger")]
    [SuppressMessage("StyleCop.CSharp.DocumentationRules", "SA1650:ElementDocumentationMustBeSpelledCorrectly", Justification = "pkgdef, VS and vsixmanifest are valid VS terms")]
    public sealed class ReCraftedPackage : Package
    {
        public const string PackageGuidString = "44fd29fa-0a2b-4384-8cba-e48f77fa6e3b";
        public const string ProjectGuid = "D04F2555-94E9-4C0F-8F39-631B6463FC2A";
        
        protected override void Initialize()
        {
            base.Initialize();

            RegisterProjectFactory(new ReCraftedProjectFactory(this));

            DebuggerLoggingService.CustomLogger = new NullLogger();
        }
    }

    class NullLogger : ICustomLogger
    {
        public string GetNewDebuggerLogFilename() => null;

        public void LogAndShowException(string message, Exception ex) { }

        public void LogError(string message, Exception ex) { }

        public void LogMessage(string messageFormat, params object[] args) { }
    }
}