// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using Microsoft.VisualStudio.Shell.Flavor;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio;
using System.Runtime.InteropServices;

namespace ReCrafted.VSDebugger
{
    internal class FlavoredProject : FlavoredProjectBase, IVsProjectFlavorCfgProvider
    {
        private IVsProjectFlavorCfgProvider _innerFlavorConfig;
        private readonly Package _package;

        public FlavoredProject(Package package)
        {
            _package = package;
        }

        public int CreateProjectFlavorCfg(IVsCfg pBaseProjectCfg, out IVsProjectFlavorCfg ppFlavorCfg)
        {
            ppFlavorCfg = null;

            if (_innerFlavorConfig != null)
            {
                GetProperty(VSConstants.VSITEMID_ROOT, (int)__VSHPROPID.VSHPROPID_ExtObject, out var project);

                _innerFlavorConfig.CreateProjectFlavorCfg(pBaseProjectCfg, out var cfg);
                ppFlavorCfg = new DebuggableConfig(cfg, project as EnvDTE.Project);
            }

            if (ppFlavorCfg != null)
                return VSConstants.S_OK;

            return VSConstants.E_FAIL;
        }

        protected override void SetInnerProject(IntPtr innerIUnknown)
        {
            var inner = Marshal.GetObjectForIUnknown(innerIUnknown);
            _innerFlavorConfig = inner as IVsProjectFlavorCfgProvider;

            if (serviceProvider == null)
                serviceProvider = _package;

            base.SetInnerProject(innerIUnknown);
        }

    }
}