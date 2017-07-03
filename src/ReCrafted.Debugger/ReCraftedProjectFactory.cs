using System.Runtime.InteropServices;
using System;
using Microsoft.VisualStudio.Shell.Flavor;

namespace ReCrafted.Debugger
{
    [ComVisible(false)]
    [Guid(ReCraftedPackage.ProjectGuid)]
    public class ReCraftedProjectFactory : FlavoredProjectFactoryBase
    {
        private ReCraftedPackage package;
        
        public ReCraftedProjectFactory(ReCraftedPackage package)
        {
            this.package = package;
        }

        protected override object PreCreateForOuter(IntPtr outerProjectIUnknown)
        {
            return new ReCraftedFlavoredProject(package);
        }
    }

}