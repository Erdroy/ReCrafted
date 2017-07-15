using Mono.Debugging.Soft;
using System.Diagnostics;
using Mono.Debugging.Client;
using Process = System.Diagnostics.Process;

namespace ReCrafted.Debugger
{
    internal class ReCraftedDebuggerSession : SoftDebuggerSession
    {
        private Process _process;

        protected override void OnRun(DebuggerStartInfo startInfo)
        {
            base.OnRun(startInfo);
            UseOperationThread = true;

            // run ReCrafted.exe file
            var rcStartInfo = new ProcessStartInfo
            {
                // TEMPORARY
                WorkingDirectory = "D:\\ReCrafted\\build\\bin64\\",
                FileName = "D:\\ReCrafted\\build\\bin64\\ReCrafted.exe",
                Arguments = "-debug"
            };

            _process = Process.Start(rcStartInfo);
        }
        
        protected override void OnExit()
        {
            _process.Kill();
        }
    }
}