// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using Microsoft.VisualStudio.OLE.Interop;
using Microsoft.VisualStudio.Shell.Interop;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio;
using Mono.Debugging.VisualStudio;
using EnvDTE;
using Mono.Debugging.Soft;
using System.Net;
using Mono.Debugging.Client;

namespace ReCrafted.VSDebugger
{
    internal class DebuggableConfig : IVsDebuggableProjectCfg, IVsProjectFlavorCfg
    {
        private IVsProjectFlavorCfg _baseProjectCfg;
        private readonly Project _baseProject;
        private MonoDebuggerLauncher _launcher;

        public DebuggableConfig(IVsProjectFlavorCfg pBaseProjectCfg, Project project)
        {
            _baseProject = project;
            _baseProjectCfg = pBaseProjectCfg;
        }

        public int Close()
        {
            if (_baseProjectCfg != null)
            {
                _baseProjectCfg.Close();
                _baseProjectCfg = null;
            }

            return VSConstants.S_OK;
        }
        
        public int DebugLaunch(uint grfLaunch)
        {
            const int port = 55000;

            var args = new SoftDebuggerConnectArgs(_baseProject.Name, IPAddress.Loopback, port)
            {
                TimeBetweenConnectionAttempts = 500,
                MaxConnectionAttempts = 4
            };
            var startInfo = new StartInfo(args, null, _baseProject);
            _launcher = new MonoDebuggerLauncher(new Progress<string>());
            _launcher.StartSession(startInfo, new ReCraftedDebuggerSession());

            return VSConstants.S_OK;
        }

        public int QueryDebugLaunch(uint grfLaunch, out int pfCanLaunch)
        {
            pfCanLaunch = 1;
            return VSConstants.S_OK;
        }

        public int get_CfgType(ref Guid iidCfg, out IntPtr ppCfg)
        {
            ppCfg = IntPtr.Zero;

            try
            {
                if (iidCfg == typeof(IVsDebuggableProjectCfg).GUID)
                    ppCfg = Marshal.GetComInterfaceForObject(this, typeof(IVsDebuggableProjectCfg));
                else if ((ppCfg == IntPtr.Zero) && (this._baseProjectCfg != null))
                    return this._baseProjectCfg.get_CfgType(ref iidCfg, out ppCfg);
            }
            catch (InvalidCastException)
            {
            }
            
            return VSConstants.S_OK;
        }

        public int EnumOutputs(out IVsEnumOutputs ppIVsEnumOutputs)
        {
            throw new NotImplementedException();
        }

        public int get_BuildableProjectCfg(out IVsBuildableProjectCfg ppIVsBuildableProjectCfg)
        {
            throw new NotImplementedException();
        }

        public int get_CanonicalName(out string pbstrCanonicalName)
        {
            throw new NotImplementedException();
        }

        public int get_DisplayName(out string pbstrDisplayName)
        {
            throw new NotImplementedException();
        }

        public int get_IsDebugOnly(out int pfIsDebugOnly)
        {
            throw new NotImplementedException();
        }

        public int get_IsPackaged(out int pfIsPackaged)
        {
            throw new NotImplementedException();
        }

        public int get_IsReleaseOnly(out int pfIsReleaseOnly)
        {
            throw new NotImplementedException();
        }

        public int get_IsSpecifyingOutputSupported(out int pfIsSpecifyingOutputSupported)
        {
            throw new NotImplementedException();
        }

        public int get_Platform(out Guid pguidPlatform)
        {
            throw new NotImplementedException();
        }

        public int get_ProjectCfgProvider(out IVsProjectCfgProvider ppIVsProjectCfgProvider)
        {
            throw new NotImplementedException();
        }

        public int get_RootURL(out string pbstrRootURL)
        {
            throw new NotImplementedException();
        }

        public int get_TargetCodePage(out uint puiTargetCodePage)
        {
            throw new NotImplementedException();
        }

        public int get_UpdateSequenceNumber(ULARGE_INTEGER[] puliUSN)
        {
            throw new NotImplementedException();
        }

        public int OpenOutput(string szOutputCanonicalName, out IVsOutput ppIVsOutput)
        {
            throw new NotImplementedException();
        }

    }
}