// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.IO;
using Mono.Debugging.Client;
using Mono.Debugging.Soft;

namespace ReCrafted.VSDebugger
{
    public class ReCraftedDebuggerSession : SoftDebuggerSession
    {
        private System.Diagnostics.Process _process;

        protected override void OnRun(DebuggerStartInfo startInfo)
        {
            base.OnRun(startInfo);

            var startupProject = ((Mono.Debugging.VisualStudio.StartInfo) startInfo).StartupProject;
            var startupProjectPath = Path.GetDirectoryName(startupProject.FileName);
            var workingDirectory = Path.GetFullPath(Path.Combine(startupProjectPath, "..", "..", "..", "Build", "Bin64"));
            var exePath = Path.Combine(workingDirectory, "ReCrafted.exe");

            _process = new System.Diagnostics.Process
            {
                StartInfo = new System.Diagnostics.ProcessStartInfo(exePath)
                {
                    WorkingDirectory = workingDirectory,
                    UseShellExecute = false,
                    Arguments = "-debug"
                }
            };
            _process.Start();
        }

        protected override void OnExit()
        {
            base.OnExit();

            if (_process != null && !_process.HasExited)
                _process.Kill();
        }
    }
}
