// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Runtime.InteropServices;
using System;
using Microsoft.VisualStudio.Shell.Flavor;

namespace ReCrafted.VSDebugger
{
    [ComVisible(false)]
    [Guid("8F3E2DF0-C35C-4265-82FC-BEA011F4A7ED")]
    public class FlavoredProjectFactory : FlavoredProjectFactoryBase
    {
        private Package _package;

        public FlavoredProjectFactory(Package package)
        {
            _package = package;
        }

        protected override object PreCreateForOuter(IntPtr outerProjectIUnknown)
        {
            return new FlavoredProject(_package);
        }
    }

}