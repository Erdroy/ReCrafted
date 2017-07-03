using System;
using Microsoft.VisualStudio.Shell.Flavor;
using Microsoft.VisualStudio.Shell.Interop;
using Microsoft.VisualStudio;
using System.Runtime.InteropServices;

namespace ReCrafted.Debugger
{
    internal class ReCraftedFlavoredProject : FlavoredProjectBase, IVsProjectFlavorCfgProvider
    {
        private IVsProjectFlavorCfgProvider innerFlavorConfig;
        private ReCraftedPackage package;

        public ReCraftedFlavoredProject(ReCraftedPackage package)
        {
            this.package = package;
        }

        public int CreateProjectFlavorCfg(IVsCfg pBaseProjectCfg, out IVsProjectFlavorCfg ppFlavorCfg)
        {
            IVsProjectFlavorCfg cfg = null;
            ppFlavorCfg = null;

            if (innerFlavorConfig != null)
            {
                object project;
                GetProperty(VSConstants.VSITEMID_ROOT, (int)__VSHPROPID.VSHPROPID_ExtObject, out project);

                this.innerFlavorConfig.CreateProjectFlavorCfg(pBaseProjectCfg, out cfg);
                ppFlavorCfg = new ReCraftedDebuggableConfig(cfg, project as EnvDTE.Project);
            }

            if (ppFlavorCfg != null)
                return VSConstants.S_OK;

            return VSConstants.E_FAIL;
        }

        protected override void SetInnerProject(IntPtr innerIUnknown)
        {
            object inner = null;

            inner = Marshal.GetObjectForIUnknown(innerIUnknown);
            innerFlavorConfig = inner as IVsProjectFlavorCfgProvider;

            if (base.serviceProvider == null)
                base.serviceProvider = this.package;

            base.SetInnerProject(innerIUnknown);
        }

    }
}